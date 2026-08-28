#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>
#include <functional>
#include "utils/animator.hpp"
#include "gameplay/buff_manager.hpp"

class Player {
private:
    std::unique_ptr<sf::Sprite> m_sprite_;
    std::unique_ptr<sf::Sprite> m_shadowSprite_;
    sf::CircleShape m_magnetCircle_;
    Animator m_animator_;

    float m_speed_;
    bool m_isAlive_;
    bool m_isMoving_;

    bool m_isRiding_ = false;
    float m_rideSpeed_ = 0.f;

    // Jump animation variables
    bool m_isJumping_ = false;
    float m_jumpProgress_ = 0.f; 
    float m_jumpDuration_ = 0.f;
    sf::Vector2f m_groundPos_;
    float m_jumpDist_ = 0.f;

    float m_tileSize_;
    int m_gridX_;
    int m_gridY_;
    sf::Vector2f m_targetPos_;

    std::function<void()> m_onDeath_;
 
public:
    Player();

    void processEvents(const std::optional<sf::Event>& event, const std::function<bool(float)>& checkChasmFunc = nullptr);
    void update(float dt);
    void render(sf::RenderWindow& window);

    int getGridX() const;
    int getGridY() const;
    void loadState(int gridX, int gridY);
    void setSkin(int skinId);

    // MOI: can thiet de CollisionSystem kiem tra va cham
    sf::FloatRect getGlobalBounds() const;
    bool isAlive() const;
    void die();

    // MOI: cho phep PlayingState (hoac bat ky ai) lang nghe su kien Player chet
    // ma khong can Player phai biet ve GameOverState/Game - giu Player doc lap (DIP).
    void setOnDeath(std::function<void()> callback);

    void setRiding(bool isRiding, float speed);

    BuffManager& getBuffManager() { return m_buffManager_; }
    const BuffManager& getBuffManager() const { return m_buffManager_; }

private:
    BuffManager m_buffManager_;
};
