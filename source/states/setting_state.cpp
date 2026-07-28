#include "states/menu_state.hpp"
#include "states/play_state.hpp"
#include "states/setting_state.hpp"
#include "core/game.hpp"
#include <iostream>

SettingState::SettingState() {
    // Khởi tạo font chữ, hình nền âm u của lâu đài...
    m_dimOverlay_.setSize({Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT});
    m_dimOverlay_.setFillColor(sf::Color(0, 0, 0, 150));

    m_popUpBox_.setSize({400.f, 300.f});
    m_popUpBox_.setFillColor(sf::Color(40, 40, 40));
    m_popUpBox_.setOutlineThickness(3.f);
    m_popUpBox_.setOutlineColor(sf::Color::White);

    centerOrigin(m_popUpBox_);
    m_popUpBox_.setPosition({Config::VIEW_WIDTH / 2, Config::VIEW_HEIGHT / 2});
}

void SettingState::processEvents(Game* game, const std::optional<sf::Event>& event) {
    if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPress->code == sf::Keyboard::Key::Escape) {
            game->popState();
            return;
        }
        if (keyPress->code == sf::Keyboard::Key::P) {
            game->changeState(new MenuState(), true);
            return;
        }
    }
}

void SettingState::update(Game* game, float dt) {
    // Hiệu ứng dơi bay lượn trên menu, chữ nhấp nháy...
}

void SettingState::render(sf::RenderWindow& window) {
    // Vẽ nút "New Game", "Load Game" ra màn hình
    window.draw(m_dimOverlay_);
    window.draw(m_popUpBox_);
}