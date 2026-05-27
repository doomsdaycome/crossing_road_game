// File: source/States/PlayingState.cpp
#include "states/MenuState.hpp"
#include "states/PlayingState.hpp"
#include "core/CGAME.hpp"
#include <iostream>

PlayingState::PlayingState() {
    // Khởi tạo font chữ, hình nền âm u của lâu đài...
}

void PlayingState::processEvents(CGAME* game, const std::optional<sf::Event>& event) {
    if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPress->code == sf::Keyboard::Key::Escape) {
            game->changeState(new MenuState());
        }
    }
}

void PlayingState::update(CGAME* game) {
    // Hiệu ứng dơi bay lượn trên menu, chữ nhấp nháy...
}

void PlayingState::render(sf::RenderWindow& window) {
    // Vẽ nút "New Game", "Load Game" ra màn hình
    window.clear(sf::Color::Red);
}