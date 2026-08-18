#pragma once

#include <SFML/Graphics.hpp>

class Animator {
private:
    int m_frameWidth_;
    int m_frameHeight_;
    int m_frameCount_;
    float m_frameDuration_;
    int m_currentFrame_;
    int m_currentDirection_;
    float m_animationTimer_;

public:
    Animator() = default;
    Animator(int frameWidth, int frameHeight, int frameCount, float frameDuration);

    void update(float dt);
    void setDirection(int direction);
    void resetToIdle();
    sf::IntRect getTextureRect() const;
};
