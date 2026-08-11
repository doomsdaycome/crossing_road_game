#include "entities/monster.hpp"
#include <cmath>

Monster::Monster(const sf::Texture& texture, float startX, float startY, float speed, int direction, int frameCount) {
    m_sprite_ = std::make_unique<sf::Sprite>(texture);

    m_speed_ = speed * direction;
    m_direction_ = direction;

    // 1. SETUP ANIMATION
    m_frameCount_ = frameCount;
    m_currentFrame_ = 0;
    m_frameDuration_ = 0.15f;
    m_animationTimer_ = 0.f;

    // Chieu rong 1 frame = Tong chieu rong anh / so luong frame
    m_frameWidth_ = static_cast<int>(texture.getSize().x) / m_frameCount_;
    m_frameHeight_ = static_cast<int>(texture.getSize().y);

    // 2. CAT FRAME DAU TIEN
    updateTextureRect();

    // 3. DAT TAM VA TOA DO
    m_sprite_->setOrigin({m_frameWidth_ / 2.f, m_frameHeight_ / 2.f});

    m_logicalPos_ = {startX, startY + (m_frameHeight_ / 2.f)};
    m_sprite_->setPosition(m_logicalPos_);
    m_joltTimer_ = 0.f;
}

void Monster::updateTextureRect() {
    int leftOffset = m_currentFrame_ * m_frameWidth_;

    if (m_direction_ == 1) {
        m_sprite_->setTextureRect(sf::IntRect({leftOffset + m_frameWidth_, 0}, {-m_frameWidth_, m_frameHeight_}));
    }
    else {
        // Lat nguoc bang cach dao chieu width
        m_sprite_->setTextureRect(sf::IntRect({leftOffset, 0}, {m_frameWidth_, m_frameHeight_}));
    }
}

void Monster::update(float deltaTime, bool isRedLight) {
    if (isRedLight) {
        // DEN DO: CHI RUNG LAC
        m_joltTimer_ += deltaTime * 50.f;
        float jitterX = std::sin(m_joltTimer_) * 3.f;
        m_sprite_->setPosition({m_logicalPos_.x + jitterX, m_logicalPos_.y});
    }
    else {
        // DEN XANH: DI TOI VA CHAY ANIMATION
        m_logicalPos_.x += m_speed_ * deltaTime;
        m_sprite_->setPosition(m_logicalPos_);
        m_joltTimer_ = 0.f;

        if (m_frameCount_ > 1) {
            m_animationTimer_ += deltaTime;
            if (m_animationTimer_ >= m_frameDuration_) {
                m_animationTimer_ -= m_frameDuration_;
                m_currentFrame_ = (m_currentFrame_ + 1) % m_frameCount_;
                updateTextureRect();
            }
        }
    }
}

void Monster::render(sf::RenderWindow& window) {
    window.draw(*m_sprite_);
}

float Monster::getPositionX() const {
    return m_logicalPos_.x;
}

bool Monster::isOffScreen() const {
    return (m_logicalPos_.x < -200.f || m_logicalPos_.x > 1224.f);
}

sf::FloatRect Monster::getGlobalBounds() const {
    return m_sprite_->getGlobalBounds();
}
