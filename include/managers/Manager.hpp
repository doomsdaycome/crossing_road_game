#pragma once

#include "managers/Config.hpp"

#include <SFML/Graphics.hpp>

class GameManager {
private:
  GameConfig game_config_ = GameConfig();
  sf::RenderWindow render_window_ =
      sf::RenderWindow(sf::VideoMode(game_config_.getWindowConfig().getSize()),
                       "Crossing Castle");

public:
  void start() {
    while (render_window_.isOpen()) {
      while (const std::optional event = render_window_.pollEvent())
        if (event->is<sf::Event::Closed>())
          render_window_.close();

      render_window_.clear();

      render_window_.display();
    }
  }
};
