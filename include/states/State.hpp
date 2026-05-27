#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class CGAME;

class State {
public:
    virtual ~State() = default;

    virtual void processEvents(CGAME* game, const std::optional<sf::Event>& event) = 0;
    virtual void update(CGAME* game) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};