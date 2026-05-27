// File: source/States/MenuState.cpp
#include "states/MenuState.hpp"
#include "states/PlayingState.hpp"
#include "core/CGAME.hpp"
#include <iostream>

MenuState::MenuState() {
    // Khởi tạo font chữ, hình nền âm u của lâu đài...
}

void MenuState::processEvents(CGAME* game, const std::optional<sf::Event>& event) {
    if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPress->code == sf::Keyboard::Key::Enter) {
            // ĐIỂM ĂN TIỀN Ở ĐÂY: Khi bấm Enter, yêu cầu CGAME đổi sang màn PlayingState
            game->changeState(new PlayingState());
        }
    }
}

void MenuState::update(CGAME* game) {
    // Hiệu ứng dơi bay lượn trên menu, chữ nhấp nháy...
}

void MenuState::render(sf::RenderWindow& window) {
    // Vẽ nút "New Game", "Load Game" ra màn hình
    window.clear(sf::Color::Black);
}