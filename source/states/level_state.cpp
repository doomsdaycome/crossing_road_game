#include "states/level_state.hpp"
#include "states/play_state.hpp"
#include "core/game.hpp"
#include <iostream>

LevelState::LevelState() {
    // Khởi tạo font chữ, hình nền âm u của lâu đài...
    if (!m_font_.openFromFile("asset/fonts/font.otf")) {
        std::cerr << "Error! Cant load font!\n";
    }

    m_level1_ = std::make_unique<sf::Text>(m_font_, "1", 50);
    centerOrigin(*m_level1_);
    m_level1_->setPosition({Config::WINDOW_WIDTH / 2, Config::WINDOW_HEIGHT / 2});
}

void LevelState::processEvents(Game* game, const std::optional<sf::Event>& event) {
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

            if (isClicked(*m_level1_, worldPos)) {
                game->changeState(new PlayingState(GameMode::CLASSIC, 1));
                return;
            }
        }
    }
}

void LevelState::update(Game* game, float dt) {
    // Hiệu ứng dơi bay lượn trên menu, chữ nhấp nháy...
    if (isMouseHovering(*m_level1_, game->getWindow())) {
        m_level1_->setFillColor(sf::Color::Red);
        m_level1_->setCharacterSize(35);
    } else {
        m_level1_->setFillColor(sf::Color::White);
        m_level1_->setCharacterSize(30);
    }
}

void LevelState::render(sf::RenderWindow& window) {
    // Vẽ nút "New Game", "Load Game" ra màn hình
    window.clear(sf::Color::Black);
    window.draw(*m_level1_);
}