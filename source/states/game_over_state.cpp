#include "states/game_over_state.hpp"
#include "states/play_state.hpp"
#include "core/game.hpp"
#include <iostream>

GameOverState::GameOverState(int finalScore) : finalScore_(finalScore) {
    std::cout << "Game Over! Diem cuoi cung: " << finalScore_ << "\n";
}

void GameOverState::processEvents(Game* game, const std::optional<sf::Event>& event) {
    if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPress->code == sf::Keyboard::Key::Enter) {
            // TODO (CHANGE PATH/VALUE): doi GameMode/level mac dinh khi choi lai
            game->changeState(new PlayingState(GameMode::CLASSIC, 1));
        }
        else if (keyPress->code == sf::Keyboard::Key::Escape) {
            game->getWindow().close();
        }
    }
}

void GameOverState::update(Game* game, float dt) {
    (void)game;
    (void)dt;
}

void GameOverState::render(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    window.clear(sf::Color(20, 20, 20));

    sf::Font font;
    // TODO (CHANGE PATH/VALUE): duong dan file font chu (xem Config::MAIN_FONT)
    if (font.openFromFile(Config::MAIN_FONT)) {
        sf::Text text(font, "Game Over - Score: " + std::to_string(finalScore_) + "\nPress ENTER to retry, ESC to quit", 28);
        text.setFillColor(sf::Color::White);
        text.setPosition({140.f, 320.f});
        window.draw(text);
    }
    else {
        std::cerr << "Loi: Khong the load font de hien thi Game Over\n";
    }
}
