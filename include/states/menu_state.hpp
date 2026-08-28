#pragma once
#include "states/state.hpp"
#include <memory>

class MenuState : public State {
private:
    std::unique_ptr<sf::Sprite> m_bgMenu_;
    std::vector<UIButton> m_buttons_;
public:
    MenuState();
    
    void processEvents(Game* game, const std::optional<sf::Event>& event) override;
    void update(Game* game, float dt) override;
    void render(sf::RenderWindow& window) override;
};