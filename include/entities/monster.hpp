#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Monster {
private:
    void updateTextureRect();

    std::unique_ptr<sf::Sprite> m_sprite_;

    float m_speed_ = 0.f;
    int m_direction_ = 1;

    // Animation
    int m_frameCount_ = 1;
    int m_currentFrame_ = 0;
    float m_frameDuration_ = 0.15f;
    float m_animationTimer_ = 0.f;
    int m_frameWidth_ = 0;
    int m_frameHeight_ = 0;

    sf::Vector2f m_logicalPos_;
    float m_joltTimer_ = 0.f;

public:
    Monster(const sf::Texture& texture, float startX, float startY, float speed, int direction, int frameCount);

    void update(float deltaTime, bool isRedLight);
    void render(sf::RenderWindow& window);

    float getPositionX() const;
    bool isOffScreen() const;

    // MOI: can thiet de CollisionSystem kiem tra va cham voi Player
    sf::FloatRect getGlobalBounds() const;
};
