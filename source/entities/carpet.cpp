#include "entities/carpet.hpp"

#include "core/config.hpp"

Carpet::Carpet(const sf::Texture& texture, float startX, float startY,
               float speed, int direction) {
  m_sprite_ = std::make_unique<sf::Sprite>(texture);

  m_speed_ = speed * direction;
  m_direction_ = direction;

  int frameWidth = texture.getSize().x;
  int frameHeight = texture.getSize().y;

  // Scale visually exactly 3 * TILE_SIZE width, 1 * TILE_SIZE height
  float scaleX = (3.f * Config::TILE_SIZE) / static_cast<float>(frameWidth);
  float scaleY = (1.f * Config::TILE_SIZE) / static_cast<float>(frameHeight);

  m_sprite_->setScale({scaleX, scaleY});
  m_sprite_->setOrigin({frameWidth / 2.f, frameHeight / 2.f});

  m_logicalPos_ = {startX, startY};
  m_sprite_->setPosition(m_logicalPos_);
}

void Carpet::update(float deltaTime, bool isRedLight) {
  // Thảm bay không dừng lại khi đèn đỏ (bởi vì đèn đỏ chỉ áp dụng cho ROAD)
  // Nhưng nếu muốn nhất quán có thể kiểm tra. Tạm thời cứ đi liên tục.
  m_logicalPos_.x += m_speed_ * deltaTime;
  m_sprite_->setPosition(m_logicalPos_);
}

void Carpet::render(sf::RenderWindow& window) { window.draw(*m_sprite_); }

float Carpet::getPositionX() const { return m_logicalPos_.x; }

bool Carpet::isOffScreen() const {
  // Magic Carpet is 3 TILE_SIZE wide (192px), we need a bit more margin to
  // delete it
  return (m_logicalPos_.x < -300.f || m_logicalPos_.x > 1324.f);
}

sf::FloatRect Carpet::getGlobalBounds() const {
  return m_sprite_->getGlobalBounds();
}

float Carpet::getSpeed() const { return m_speed_; }
