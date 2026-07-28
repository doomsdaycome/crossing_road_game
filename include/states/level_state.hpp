#pragma once
#include "states/state.hpp"
#include <memory>

class LevelState : public State {
private:
    sf::Font m_font_;
    std::unique_ptr<sf::Text> m_level1_;
public:
    LevelState();
    
    void processEvents(Game* game, const std::optional<sf::Event>& event) override;
    void update(Game* game, float dt) override;
    void render(sf::RenderWindow& window) override;
};