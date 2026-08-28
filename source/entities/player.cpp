#include "entities/player.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "core/config.hpp"
#include "services/resource_manager.hpp"
#include "utils/utils.hpp"

Player::Player()
    : m_speed_(Config::PLAYER_SPEED), m_isAlive_(true), m_isMoving_(false) {
  m_tileSize_ = Config::TILE_SIZE;
  m_gridX_ = Config::PLAYER_START_GRID_X;
  m_gridY_ = Config::PLAYER_START_GRID_Y;

  // Tính toán tọa độ tâm của ô
  m_targetPos_ = {m_gridX_ * m_tileSize_ + (m_tileSize_ / 2),
                  m_gridY_ * m_tileSize_ + (m_tileSize_ / 2)};

  // Lấy texture đã cache từ ResourceManager với mask trong suốt (Magenta)
  const sf::Texture& texture = ResourceManager::instance().getTextureWithMask(
      Config::PLAYER_TEXTURE, sf::Color::Magenta);

  m_sprite_ = std::make_unique<sf::Sprite>(texture);

  int frameWidth = texture.getSize().x / 4;
  int frameHeight = texture.getSize().y / 4;
  m_animator_ = Animator(frameWidth, frameHeight, 4, 0.15f);
  m_sprite_->setTextureRect(m_animator_.getTextureRect());

  // Scale the sprite so the frame width shrinks down to fit TILE_SIZE exactly
  float scale = Config::TILE_SIZE / static_cast<float>(frameWidth);
  m_sprite_->setScale({scale, scale});

  // Căn giữa tâm sprite dựa trên kích thước 1 frame
  m_sprite_->setOrigin({frameWidth / 2.f, frameHeight / 2.f});
  m_sprite_->setPosition(m_targetPos_);

  m_groundPos_ = m_targetPos_;

  // Khởi tạo cái bóng (Shadow)
  // Dùng 1 vòng tròn dẹt màu đen mờ
  // Do SFML không có hàm vẽ hình elip, có thể dùng CircleShape + scale Y
  // Nhưng vì Shadow có thể phức tạp, ta tạo thẳng 1 texture mờ mờ hoặc đơn giản
  // dùng sf::Sprite nếu có texture bóng. Nếu không có, ta không vẽ gì hoặc tạo
  // 1 sf::CircleShape (cần include, nhưng prompt bảo dùng m_shadowSprite_). Tôi
  // sẽ gán nó bằng texture hiện tại nhưng tô màu đen và ép bẹp lại
  m_shadowSprite_ = std::make_unique<sf::Sprite>(texture);
  m_shadowSprite_->setTextureRect(m_animator_.getTextureRect());
  m_shadowSprite_->setColor(sf::Color(0, 0, 0, 100));  // Đen mờ
  m_shadowSprite_->setOrigin({frameWidth / 2.f, frameHeight / 2.f});
  m_shadowSprite_->setPosition(m_targetPos_);
  m_shadowSprite_->setScale({scale, scale * 0.3f});  // Ép bẹp xuống thành bóng

  // Magnet circle effect
  m_magnetCircle_.setRadius(Config::TILE_SIZE *
                            3.0f);  // Bán kính vòng nam châm
  m_magnetCircle_.setFillColor(
      sf::Color(255, 215, 0, 40));  // Vàng trong suốt mờ
  m_magnetCircle_.setOutlineColor(
      sf::Color(255, 255, 0, 150));  // Viền vàng sáng
  m_magnetCircle_.setOutlineThickness(2.f);
  m_magnetCircle_.setOrigin(
      {m_magnetCircle_.getRadius(), m_magnetCircle_.getRadius()});
  m_magnetCircle_.setPosition(m_targetPos_);
}

void Player::processEvents(const std::optional<sf::Event>& event,
                           const std::function<bool(float)>& checkChasmFunc) {
  // 1. Kiểm tra trạng thái: Chết hoặc đang bay trên không thì bỏ qua toàn bộ
  // phím bấm
  if (!m_isAlive_) return;

  // FIX: Xóa đoạn code teleport cũ. Giờ đang di chuyển là KHÔNG nhận phím!
  if (m_isMoving_) return;

  if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
    bool hasInput = false;

    // XÓA GRID SNAPPING: Không ép teleport tọa độ trước khi nhảy để giữ chuyển
    // động mượt mà

    int nextGridX = m_gridX_;
    int nextGridY = m_gridY_;

    // 2. Bắt hướng di chuyển
    if (keyPress->code == sf::Keyboard::Key::W ||
        keyPress->code == sf::Keyboard::Key::Up) {
      nextGridY -= 1;
      m_animator_.setDirection(3);  // Up
      hasInput = true;
    } else if (keyPress->code == sf::Keyboard::Key::S ||
               keyPress->code == sf::Keyboard::Key::Down) {
      nextGridY += 1;
      m_animator_.setDirection(0);  // Down
      hasInput = true;
    } else if (keyPress->code == sf::Keyboard::Key::A ||
               keyPress->code == sf::Keyboard::Key::Left) {
      nextGridX -= 1;
      m_animator_.setDirection(1);  // Left
      hasInput = true;
    } else if (keyPress->code == sf::Keyboard::Key::D ||
               keyPress->code == sf::Keyboard::Key::Right) {
      nextGridX += 1;
      m_animator_.setDirection(2);  // Right
      hasInput = true;
    }

    // 3. Khóa biên màn hình để không chạy ra ngoài (Giữ nguyên logic cực chuẩn
    // của ông)
    nextGridX = std::clamp(nextGridX, Config::PLAYER_MIN_GRID_X,
                           Config::PLAYER_MAX_GRID_X);

    // 4. Nếu có phím hợp lệ và thực sự có sự thay đổi ô grid
    if (hasInput && (nextGridX != m_gridX_ || nextGridY != m_gridY_)) {
      m_gridX_ = nextGridX;
      m_gridY_ = nextGridY;

      // Cập nhật đích đến
      m_targetPos_ = {m_gridX_ * m_tileSize_ + (m_tileSize_ / 2),
                      m_gridY_ * m_tileSize_ + (m_tileSize_ / 2)};

      // Bật cờ khóa phím, bắt đầu hành trình lết tới đích
      m_isMoving_ = true;
      m_jumpProgress_ = 0.f;

      // Tính toán khoảng cách tổng để vẽ Parabol (Sine wave)
      float dx = m_targetPos_.x - m_groundPos_.x;
      float dy = m_targetPos_.y - m_groundPos_.y;
      m_jumpDist_ = std::sqrt(dx * dx + dy * dy);
      m_jumpDuration_ = m_jumpDist_ / m_speed_;

      // QUYẾT ĐỊNH JUMP: Chỉ nhảy (2.5D) nếu đang ở CHASM (đi ra) hoặc target
      // là CHASM (đi vào)
      bool targetIsChasm = checkChasmFunc
                               ? checkChasmFunc(nextGridY * Config::TILE_SIZE)
                               : false;
      m_isJumping_ = m_isRiding_ || targetIsChasm;

      // Gỡ cờ cưỡi thảm ngay khi bắt đầu bật nhảy (để tránh bị trôi tiếp lúc
      // đang trên không)
      m_isRiding_ = false;

      // [Gợi ý] Ông có thể mở comment dòng này để có tiếng lết/nhảy
      // ResourceManager::instance().playSound("sfx_jump");
    }
  }
}

void Player::update(float dt) {
  m_buffManager_.update(dt);

  // Apply Phantom Cloak visual effect
  if (m_buffManager_.isInvisible()) {
    m_sprite_->setColor(sf::Color(255, 255, 255, 128));  // Lower alpha
  } else {
    m_sprite_->setColor(sf::Color::White);  // Normal
  }

  // Update Magnet Circle
  if (m_buffManager_.isMagnetActive()) {
    m_magnetCircle_.setPosition(m_groundPos_);  // Đi theo người chơi

    // Hiệu ứng nhịp đập mờ mờ cho nam châm
    static float magnetPulseTime = 0.f;
    magnetPulseTime += dt;
    float pulseScale = 1.0f + 0.05f * std::sin(magnetPulseTime * 5.f);
    m_magnetCircle_.setScale({pulseScale, pulseScale});
  }

  if (m_isMoving_) {
    m_animator_.update(dt);
    m_shadowSprite_->setTextureRect(m_animator_.getTextureRect());
  } else {
    m_animator_.resetToIdle();
    m_shadowSprite_->setTextureRect(m_animator_.getTextureRect());
  }
  m_sprite_->setTextureRect(m_animator_.getTextureRect());

  // Nếu đang đứng yên thì khỏi tính toán mất công
  if (!m_isMoving_) {
    // Cập nhật vị trí trôi nếu đang đứng trên thảm bay
    if (m_isRiding_) {
      m_groundPos_.x += m_rideSpeed_ * dt;
      m_sprite_->setPosition(m_groundPos_);
      m_shadowSprite_->setPosition(m_groundPos_);

      // Cập nhật gridX liên tục theo tọa độ trôi để khóa camera/màn hình mượt
      // mà
      m_gridX_ = static_cast<int>(m_groundPos_.x / Config::TILE_SIZE);
      m_targetPos_ = m_groundPos_;
    }
    return;
  }

  // Tính vector hướng và khoảng cách theo tọa độ GỐC (mặt đất)
  float dx = m_targetPos_.x - m_groundPos_.x;
  float dy = m_targetPos_.y - m_groundPos_.y;
  float distance = std::sqrt(dx * dx + dy * dy);

  // Quãng đường đi được trong frame này (đã được làm chậm nếu mắt mở)
  float moveStep = m_speed_ * dt;

  if (distance <= moveStep) {
    // ĐÃ TỚI ĐÍCH: Snap cho khớp ô và mở khóa phím
    m_groundPos_ = m_targetPos_;
    m_sprite_->setPosition(m_groundPos_);
    m_shadowSprite_->setPosition(m_groundPos_);
    m_shadowSprite_->setScale(
        {m_sprite_->getScale().x, m_sprite_->getScale().x * 0.3f});

    m_isMoving_ = false;
    m_jumpProgress_ = 0.f;
  } else {
    // ĐANG TRÊN ĐƯỜNG: Đi tiếp theo tỷ lệ khoảng cách
    float moveX = (dx / distance) * moveStep;
    float moveY = (dy / distance) * moveStep;
    m_groundPos_.x += moveX;
    m_groundPos_.y += moveY;

    if (m_isJumping_) {
      // Cập nhật Parabol (Sine Wave) cho bước nhảy 2.5D
      m_jumpProgress_ += dt;
      float ratio = std::clamp(m_jumpProgress_ / m_jumpDuration_, 0.f, 1.f);

      // Hình sin cung cấp độ cong hoàn hảo từ 0 -> 1 -> 0
      float jumpArc = std::sin(ratio * 3.14159265f);
      float jumpHeight = 30.f;  // Độ cao cực đại của cú nhảy (pixels)

      // Cập nhật vị trí Sprite: Y bị đẩy lên cao bởi jumpArc
      sf::Vector2f visualPos = m_groundPos_;
      visualPos.y -= jumpArc * jumpHeight;
      m_sprite_->setPosition(visualPos);

      // Cập nhật Shadow: Đứng dưới mặt đất, nhưng bị thu nhỏ lại khi bay lên
      // cao
      m_shadowSprite_->setPosition(m_groundPos_);
      float scale = m_sprite_->getScale().x;
      float shadowShrink = 1.0f - (jumpArc * 0.4f);  // Bóng nhỏ lại tối đa 40%
      m_shadowSprite_->setScale(
          {scale * shadowShrink, scale * 0.3f * shadowShrink});
    } else {
      // Đi bộ bình thường, không nảy, bóng nguyên kích thước
      m_sprite_->setPosition(m_groundPos_);
      m_shadowSprite_->setPosition(m_groundPos_);
    }
  }
}

void Player::render(sf::RenderWindow& window) {
  if (m_isAlive_) {
    // Vẽ vòng tròn nam châm nếu đang kích hoạt (Vẽ nằm dưới player)
    if (m_buffManager_.isMagnetActive()) {
      window.draw(m_magnetCircle_);
    }

    // Chi ve bong neu dang jump de nhat quan, hoac luon ve cung duoc. User bảo
    // "or shadow effect", Tức là khi đi bộ thì KHÔNG có shadow.
    if (m_isJumping_ && m_isMoving_) {
      window.draw(*m_shadowSprite_);
    }
    window.draw(*m_sprite_);
  }
}

int Player::getGridX() const { return m_gridX_; }
int Player::getGridY() const { return m_gridY_; }

void Player::loadState(int gridX, int gridY) {
  m_gridX_ = gridX;
  m_gridY_ = gridY;

  m_targetPos_ = {m_gridX_ * m_tileSize_ + (m_tileSize_ / 2),
                  m_gridY_ * m_tileSize_ + (m_tileSize_ / 2)};
  m_groundPos_ = m_targetPos_;
  m_sprite_->setPosition(m_groundPos_);
  m_shadowSprite_->setPosition(m_groundPos_);
  m_isMoving_ = false;
}

sf::FloatRect Player::getGlobalBounds() const {
  sf::FloatRect bounds = m_sprite_->getGlobalBounds();

  // Hitbox bất biến: Dìm hitbox xuống lại mặt đất (m_groundPos_)
  // thay vì bay lên không trung cùng với ảnh Sprite.
  float visualY = m_sprite_->getPosition().y;
  float groundY = m_groundPos_.y;
  bounds.position.y += (groundY - visualY);

  return bounds;
}

bool Player::isAlive() const { return m_isAlive_; }

void Player::die() {
  // Khóa chống spam callback (ví dụ va chạm quái nhiều lần trong 1 frame)
  if (!m_isAlive_) return;

  m_isAlive_ = false;
  if (m_onDeath_) {
    m_onDeath_();
  }
}

void Player::setOnDeath(std::function<void()> callback) {
  m_onDeath_ = std::move(callback);
}

void Player::setRiding(bool isRiding, float speed) {
  m_isRiding_ = isRiding;
  m_rideSpeed_ = speed;
}

void Player::setSkin(int skinId) {
  std::string texPath =
      "asset/images/entities/player/pl" + std::to_string(skinId) + ".png";
  const sf::Texture& texture = ResourceManager::instance().getTextureWithMask(
      texPath, sf::Color::Magenta);
  m_sprite_->setTexture(texture);
}
