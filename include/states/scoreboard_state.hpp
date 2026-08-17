#pragma once
#include "states/state.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class ScoreboardState : public State {
public:
    ScoreboardState();
    
    void update(Game* game, float dt) override;
    void render(sf::RenderWindow& window) override;
    void processEvents(Game* game, const std::optional<sf::Event>& event) override;

private:
    std::unique_ptr<sf::Sprite> m_bgSprite_; // Đã đổi thành Sprite để load ảnh
    
    std::unique_ptr<sf::Text> m_titleText_;
    std::unique_ptr<sf::Text> m_bestScoreText_;
    std::unique_ptr<sf::Text> m_totalGoldText_;
    std::unique_ptr<sf::Text> m_rankText_; 
    
    std::unique_ptr<sf::Sprite> m_backBtnSprite_;
    std::unique_ptr<sf::Text> m_backBtnText_;
};