// File: include/States/MenuState.hpp
#pragma once
#include "states/State.hpp"

class MenuState : public State {
public:
    MenuState();
    
    void processEvents(CGAME* game, const std::optional<sf::Event>& event) override;
    void update(CGAME* game) override;
    void render(sf::RenderWindow& window) override;
};