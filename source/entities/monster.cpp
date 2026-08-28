#include "entities/monster.hpp"

#include <cmath>

#include "core/config.hpp"

Monster::Monster(const sf::Texture& texture, float startX, float startY,
                 float speed, int direction, int frameCount)
    : m_animator_(texture.getSize().x / frameCount, texture.getSize().y / 2,
                  frameCount, 0.15f) {
  m_sprite_ = std::make_unique<sf::Sprite>(texture);

  m_speed_ = speed * direction;
  m_direction_ = direction;

  int frameWidth = texture.getSize().x / frameCount;
  int frameHeight = texture.getSize().y / 2;  // Sprite sheet is 2 rows

  // Map direction to the correct row (e.g., 0 for Left/Backward, 1 for
  // Right/Forward)
  int row = (direction == 1) ? 1 : 0;
  m_animator_.setDirection(row);

  m_sprite_->setTextureRect(m_animator_.getTextureRect());

  // Thu nhỏ quái vật để vừa với 1 ô lưới
  float scale = Config::TILE_SIZE / static_cast<float>(frameWidth);
  m_sprite_->setScale({scale, scale});

  // 3. DAT TAM VA TOA DO
  m_sprite_->setOrigin({frameWidth / 2.f, frameHeight / 2.f});

  // startY đã được truyền từ Lane là yPosition_ + TILE_SIZE / 2.f (tức là đã ở
  // chính giữa)
  m_logicalPos_ = {startX, startY};
  m_sprite_->setPosition(m_logicalPos_);
  m_joltTimer_ = 0.f;
}

void Monster::update(float deltaTime, bool isRedLight) {
  if (isRedLight) {
    // DEN DO: CHI RUNG LAC
    m_joltTimer_ += deltaTime * 50.f;
    float jitterX = std::sin(m_joltTimer_) * 3.f;
    m_sprite_->setPosition({m_logicalPos_.x + jitterX, m_logicalPos_.y});
  } else {
    // DEN XANH: DI TOI VA CHAY ANIMATION
    m_logicalPos_.x += m_speed_ * deltaTime;
    m_sprite_->setPosition(m_logicalPos_);
    m_joltTimer_ = 0.f;

    m_animator_.update(deltaTime);
    m_sprite_->setTextureRect(m_animator_.getTextureRect());
  }
}

void Monster::render(sf::RenderWindow& window) { window.draw(*m_sprite_); }

float Monster::getPositionX() const { return m_logicalPos_.x; }

bool Monster::isOffScreen() const {
  return (m_logicalPos_.x < -200.f || m_logicalPos_.x > 1224.f);
}

sf::FloatRect Monster::getGlobalBounds() const {
  return m_sprite_->getGlobalBounds();
}
