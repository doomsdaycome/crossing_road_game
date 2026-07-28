#include "states/menu_state.hpp"
#include "states/mode_state.hpp"
#include "core/game.hpp"
#include <iostream>

MenuState::MenuState() {
    // Khởi tạo font chữ, hình nền âm u của lâu đài...
    if (!m_font_.openFromFile("asset/fonts/font.otf")) {
        std::cerr << "Error! Cant load font!\n";
    }

    m_startButton_= std::make_unique<sf::Text>(m_font_, "Start Game", 30);
    m_startButton_->setPosition({100.f, 200.f});
}

void MenuState::processEvents(Game* game, const std::optional<sf::Event>& event) {
    if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>());
    if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2i pixelPos = {mouseEvent->position.x, mouseEvent->position.y};
            sf::Vector2f worldPos = game->getWindow().mapPixelToCoords(pixelPos);

            if (isClicked(*m_startButton_, worldPos)) {
                game->pushState(new ModeState());
                return;
            }
        }
    }
}

void MenuState::update(Game* game, float dt) {
    // Hiệu ứng dơi bay lượn trên menu, chữ nhấp nháy...
    if (isMouseHovering(*m_startButton_, game->getWindow())) {
        m_startButton_->setFillColor(sf::Color::Red);
        m_startButton_->setCharacterSize(35);
    } else {
        m_startButton_->setFillColor(sf::Color::White);
        m_startButton_->setCharacterSize(30);
    }
}

void MenuState::render(sf::RenderWindow& window) {
    // Vẽ nút "New Game", "Load Game" ra màn hình
    window.clear(sf::Color::Black);
    window.draw(*m_startButton_);
}