#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Carpet {
 private:
  std::unique_ptr<sf::Sprite> m_sprite_;

  float m_speed_ = 0.f;
  int m_direction_ = 1;

  sf::Vector2f m_logicalPos_;

 public:
  Carpet(const sf::Texture& texture, float startX, float startY, float speed,
         int direction);

  void update(float deltaTime, bool isRedLight);
  void render(sf::RenderWindow& window);

  float getPositionX() const;
  bool isOffScreen() const;
  sf::FloatRect getGlobalBounds() const;

  float getSpeed() const;
};
