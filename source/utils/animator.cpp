#include "utils/animator.hpp"

Animator::Animator(int frameWidth, int frameHeight, int frameCount, float frameDuration)
    : m_frameWidth_(frameWidth),
      m_frameHeight_(frameHeight),
      m_frameCount_(frameCount),
      m_frameDuration_(frameDuration),
      m_currentFrame_(0),
      m_currentDirection_(0),
      m_animationTimer_(0.f) {}

void Animator::update(float dt) {
    if (m_frameCount_ <= 1) return;

    m_animationTimer_ += dt;
    if (m_animationTimer_ >= m_frameDuration_) {
        m_animationTimer_ -= m_frameDuration_;
        m_currentFrame_ = (m_currentFrame_ + 1) % m_frameCount_;
    }
}

void Animator::setDirection(int direction) {
    if (m_currentDirection_ != direction) {
        m_currentDirection_ = direction;
        m_currentFrame_ = 0;
        m_animationTimer_ = 0.f;
    }
}

void Animator::resetToIdle() {
    m_currentFrame_ = 0;
    m_animationTimer_ = 0.f;
}

sf::IntRect Animator::getTextureRect() const {
    int leftOffset = m_currentFrame_ * m_frameWidth_;
    int topOffset = m_currentDirection_ * m_frameHeight_;
    
    // For directions that might flip the sprite, we'd handle it here.
    // Assuming standard 4-dir sprite sheet (0: Down, 1: Left, 2: Right, 3: Up)
    // Or just simple row-based.
    // If direction is 1 (Left) and we need to flip, we might do so.
    // But typically 4-row spritesheet doesn't flip, it has dedicated rows.
    // Wait, the monster logic did:
    // if (m_direction_ == 1) leftOffset + m_frameWidth_, -m_frameWidth_
    // else leftOffset, m_frameWidth_
    // We should accommodate standard or parameterize it.
    // For now, let's just return the standard Rect.
    // Let's implement standard rect, and we will tweak it if needed.
    return sf::IntRect({leftOffset, topOffset}, {m_frameWidth_, m_frameHeight_});
}
