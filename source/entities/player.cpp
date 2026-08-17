#include "entities/player.hpp"
#include "services/resource_manager.hpp"
#include "core/config.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

Player::Player() : m_speed_(Config::PLAYER_SPEED), m_isAlive_(true), m_isMoving_(false) {
    m_tileSize_ = Config::TILE_SIZE;
    m_gridX_ = Config::PLAYER_START_GRID_X;
    m_gridY_ = Config::PLAYER_START_GRID_Y;

    // Tính toán tọa độ tâm của ô
    m_targetPos_ = {
        m_gridX_ * m_tileSize_ + (m_tileSize_ / 2),
        m_gridY_ * m_tileSize_ + (m_tileSize_ / 2)
    };

    // Lấy texture đã cache từ ResourceManager
    const sf::Texture& texture = ResourceManager::instance().getTexture(Config::PLAYER_TEXTURE);

    m_sprite_ = std::make_unique<sf::Sprite>(texture);
    centerOrigin(*m_sprite_);
    m_sprite_->setPosition(m_targetPos_);
}

void Player::processEvents(const std::optional<sf::Event>& event) {
    // 1. Kiểm tra trạng thái: Chết hoặc đang bay trên không thì bỏ qua toàn bộ phím bấm
    if (!m_isAlive_) return; 
    
    // FIX: Xóa đoạn code teleport cũ. Giờ đang di chuyển là KHÔNG nhận phím!
    if (m_isMoving_) return; 

    if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
        bool hasInput = false;
        int nextGridX = m_gridX_;
        int nextGridY = m_gridY_;

        // 2. Bắt hướng di chuyển
        if (keyPress->code == sf::Keyboard::Key::W || keyPress->code == sf::Keyboard::Key::Up) {
            nextGridY -= 1;
            hasInput = true;
        }
        else if (keyPress->code == sf::Keyboard::Key::S || keyPress->code == sf::Keyboard::Key::Down) {
            nextGridY += 1;
            hasInput = true;
        }
        else if (keyPress->code == sf::Keyboard::Key::A || keyPress->code == sf::Keyboard::Key::Left) {
            nextGridX -= 1;
            hasInput = true;
        }
        else if (keyPress->code == sf::Keyboard::Key::D || keyPress->code == sf::Keyboard::Key::Right) {
            nextGridX += 1;
            hasInput = true;
        }

        // 3. Khóa biên màn hình để không chạy ra ngoài (Giữ nguyên logic cực chuẩn của ông)
        nextGridX = std::clamp(nextGridX, Config::PLAYER_MIN_GRID_X, Config::PLAYER_MAX_GRID_X);

        // 4. Nếu có phím hợp lệ và thực sự có sự thay đổi ô grid
        if (hasInput && (nextGridX != m_gridX_ || nextGridY != m_gridY_)) {
            m_gridX_ = nextGridX;
            m_gridY_ = nextGridY;
            
            // Cập nhật đích đến
            m_targetPos_ = {
                m_gridX_ * m_tileSize_ + (m_tileSize_ / 2),
                m_gridY_ * m_tileSize_ + (m_tileSize_ / 2)
            };
            
            // Bật cờ khóa phím, bắt đầu hành trình lết tới đích
            m_isMoving_ = true;
            
            // [Gợi ý] Ông có thể mở comment dòng này để có tiếng lết/nhảy
            // ResourceManager::instance().playSound("sfx_jump");
        }
    }
}

void Player::update(float dt) {
    // Nếu đang đứng yên thì khỏi tính toán mất công
    if (!m_isMoving_) return; 

    sf::Vector2f currentPos = m_sprite_->getPosition();

    // Tính vector hướng và khoảng cách
    float dx = m_targetPos_.x - currentPos.x;
    float dy = m_targetPos_.y - currentPos.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    // Quãng đường đi được trong frame này (đã được làm chậm nếu mắt mở)
    float moveStep = m_speed_ * dt;

    if (distance <= moveStep) {
        // ĐÃ TỚI ĐÍCH: Snap cho khớp ô và mở khóa phím
        m_sprite_->setPosition(m_targetPos_);
        m_isMoving_ = false;
    }
    else {
        // ĐANG TRÊN ĐƯỜNG: Đi tiếp theo tỷ lệ khoảng cách
        float moveX = (dx / distance) * moveStep;
        float moveY = (dy / distance) * moveStep;
        m_sprite_->move({moveX, moveY});
    }
}

void Player::render(sf::RenderWindow& window) {
    if (m_isAlive_) {
        window.draw(*m_sprite_);
    }
}

int Player::getGridX() const { return m_gridX_; }
int Player::getGridY() const { return m_gridY_; }

void Player::loadState(int gridX, int gridY) {
    m_gridX_ = gridX;
    m_gridY_ = gridY;

    m_targetPos_ = {
        m_gridX_ * m_tileSize_ + (m_tileSize_ / 2),
        m_gridY_ * m_tileSize_ + (m_tileSize_ / 2)
    };
    m_sprite_->setPosition(m_targetPos_);
    m_isMoving_ = false;
}

sf::FloatRect Player::getGlobalBounds() const {
    return m_sprite_->getGlobalBounds();
}

bool Player::isAlive() const {
    return m_isAlive_;
}

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
