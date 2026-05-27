// File: include/States/PlayingState.hpp
#pragma once
#include "states/State.hpp"

class PlayingState : public State {
public:
    PlayingState();
    
    void processEvents(CGAME* game, const std::optional<sf::Event>& event) override;
    void update(CGAME* game) override;
    void render(sf::RenderWindow& window) override;
};