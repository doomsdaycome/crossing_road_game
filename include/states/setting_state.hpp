#pragma once
#include "states/state.hpp"
#include <memory>

class SettingState : public State {
private:
    // sf::Font m_font_;
    // std::unique_ptr<sf::Text> m_startButton_;

    sf::RectangleShape m_dimOverlay_, m_popUpBox_;
public:
    SettingState();
    
    void processEvents(Game* game, const std::optional<sf::Event>& event) override;
    void update(Game* game, float dt) override;
    void render(sf::RenderWindow& window) override;
};