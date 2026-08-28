#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "utils/animator.hpp"

class Monster {
 private:
  std::unique_ptr<sf::Sprite> m_sprite_;
  Animator m_animator_;

  float m_speed_ = 0.f;
  int m_direction_ = 1;

  sf::Vector2f m_logicalPos_;
  float m_joltTimer_ = 0.f;

 public:
  Monster(const sf::Texture& texture, float startX, float startY, float speed,
          int direction, int frameCount);

  void update(float deltaTime, bool isRedLight);
  void render(sf::RenderWindow& window);

  float getPositionX() const;
  bool isOffScreen() const;

  // MOI: can thiet de CollisionSystem kiem tra va cham voi Player
  sf::FloatRect getGlobalBounds() const;
};
