#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>
#include <functional>

// Ham tien ich: dat origin cua sprite vao chinh giua (dung o nhieu entity)
void centerOrigin(sf::Sprite& sprite);

class Player {
private:
    std::unique_ptr<sf::Sprite> m_sprite_;

    float m_speed_;
    bool m_isAlive_;
    bool m_isMoving_;

    float m_tileSize_;
    int m_gridX_;
    int m_gridY_;
    sf::Vector2f m_targetPos_;

    std::function<void()> m_onDeath_;
 
public:
    Player();

    void processEvents(const std::optional<sf::Event>& event);
    void update(float dt);
    void render(sf::RenderWindow& window);

    int getGridX() const;
    int getGridY() const;
    void loadState(int gridX, int gridY);

    // MOI: can thiet de CollisionSystem kiem tra va cham
    sf::FloatRect getGlobalBounds() const;
    bool isAlive() const;
    void die();

    // MOI: cho phep PlayingState (hoac bat ky ai) lang nghe su kien Player chet
    // ma khong can Player phai biet ve GameOverState/Game - giu Player doc lap (DIP).
    void setOnDeath(std::function<void()> callback);
};
