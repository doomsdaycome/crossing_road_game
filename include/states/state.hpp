#pragma once
#include <SFML/Graphics.hpp>
#include "utils/utils.hpp"
#include <optional>

class Game;

class State {
public:
    virtual ~State() = default;

    virtual void processEvents(Game* game, const std::optional<sf::Event>& event) = 0;
    virtual void update(Game* game, float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};