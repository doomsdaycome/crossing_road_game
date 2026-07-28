#include "entities/player.hpp"
#include "services/resource_manager.hpp"
#include "core/config.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

void centerOrigin(sf::Sprite& sprite) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
}

Player::Player() : m_speed_(Config::PLAYER_SPEED), m_isAlive_(true), m_isMoving_(false) {
    m_tileSize_ = Config::TILE_SIZE;
    m_gridX_ = Config::PLAYER_START_GRID_X;
    m_gridY_ = Config::PLAYER_START_GRID_Y;

    m_targetPos_ = {
        m_gridX_ * m_tileSize_ + (m_tileSize_ / 2),
        m_gridY_ * m_tileSize_ + (m_tileSize_ / 2)
    };

    // FIX: khong con tu goi loadFromFile() - lay texture da cache tu ResourceManager
    const sf::Texture& texture = ResourceManager::instance().getTexture(Config::PLAYER_TEXTURE);

    m_sprite_ = std::make_unique<sf::Sprite>(texture);
    centerOrigin(*m_sprite_);
    m_sprite_->setPosition(m_targetPos_);
}

void Player::processEvents(const std::optional<sf::Event>& event) {
    if (!m_isAlive_) return; // Chet roi thi mien thao tac

    if (!m_isMoving_) {
        if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
            bool hasInput = false;
            int nextGridX = m_gridX_;
            int nextGridY = m_gridY_;

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

            // FIX: gioi han bien ban do theo chieu ngang (ban goc co the di ra ngoai vo han)
            nextGridX = std::clamp(nextGridX, Config::PLAYER_MIN_GRID_X, Config::PLAYER_MAX_GRID_X);

            if (hasInput && (nextGridX != m_gridX_ || nextGridY != m_gridY_)) {
                m_gridX_ = nextGridX;
                m_gridY_ = nextGridY;
                m_targetPos_ = {
                    m_gridX_ * m_tileSize_ + (m_tileSize_ / 2),
                    m_gridY_ * m_tileSize_ + (m_tileSize_ / 2)
                };
                m_isMoving_ = true;
            }
        }
    }
}

void Player::update(float dt) {
    if (m_isMoving_) {
        sf::Vector2f currentPos = m_sprite_->getPosition();

        float dx = m_targetPos_.x - currentPos.x;
        float dy = m_targetPos_.y - currentPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        float moveStep = m_speed_ * dt;

        if (distance > moveStep) {
            float moveX = (dx / distance) * moveStep;
            float moveY = (dy / distance) * moveStep;

            m_sprite_->move({moveX, moveY});
        }
        else {
            m_sprite_->setPosition(m_targetPos_);
            m_isMoving_ = false;
        }
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
    if (!m_isAlive_) return; // tranh goi callback nhieu lan neu die() bi goi lap
    m_isAlive_ = false;
    if (m_onDeath_) {
        m_onDeath_();
    }
}

void Player::setOnDeath(std::function<void()> callback) {
    m_onDeath_ = std::move(callback);
}
