#include "states/mode_state.hpp"
#include "states/level_state.hpp"
#include "states/play_state.hpp"
#include "core/game.hpp"
#include <iostream>

ModeState::ModeState() {
    // Khởi tạo font chữ, hình nền âm u của lâu đài...
    if (!m_font_.openFromFile("asset/fonts/font.otf")) {
        std::cerr << "Error! Cant load font!\n";
    }
    
    m_classicMode_ = std::make_unique<sf::Text>(m_font_, "Classic", 30);
    m_endlessMode_ = std::make_unique<sf::Text>(m_font_, "Endless", 30);
    centerOrigin(*m_classicMode_);
    centerOrigin(*m_endlessMode_);
    m_classicMode_->setPosition({Config::WINDOW_WIDTH / 2, 300});
    m_endlessMode_->setPosition({Config::WINDOW_HEIGHT / 2, 600});
}

void ModeState::processEvents(Game* game, const std::optional<sf::Event>& event) {
    if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPress->code == sf::Keyboard::Key::Escape) {
            game->popState();
            return;
        }
    }
    if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2i pixelPos = {mouseEvent->position.x, mouseEvent->position.y};
            sf::Vector2f worldPos = game->getWindow().mapPixelToCoords(pixelPos);

            if (isClicked(*m_classicMode_, worldPos)) {
                game->pushState(new LevelState());
                return;
            }
            else if (isClicked(*m_endlessMode_, worldPos)) {
                game->changeState(new PlayingState(GameMode::ENDLESS));
                return;
            }
        }
    }
}

void ModeState::update(Game* game, float dt) {
    // Hiệu ứng dơi bay lượn trên menu, chữ nhấp nháy...
    if (isMouseHovering(*m_classicMode_, game->getWindow())) {
        m_classicMode_->setFillColor(sf::Color::Red);
        m_classicMode_->setCharacterSize(35);
    } else {
        m_classicMode_->setFillColor(sf::Color::White);
        m_classicMode_->setCharacterSize(30);
    }
    if (isMouseHovering(*m_endlessMode_, game->getWindow())) {
        m_endlessMode_->setFillColor(sf::Color::Red);
        m_endlessMode_->setCharacterSize(35);
    } else {
        m_endlessMode_->setFillColor(sf::Color::White);
        m_endlessMode_->setCharacterSize(30);
    }
}

void ModeState::render(sf::RenderWindow& window) {
    // Vẽ nút "New Game", "Load Game" ra màn hình
    window.clear(sf::Color::Black);
    window.draw(*m_classicMode_);
    window.draw(*m_endlessMode_);
}