#include "states/menu_state.hpp"
#include "states/mode_state.hpp"
#include "states/setting_state.hpp"
#include "states/load_state.hpp"
#include "states/scoreboard_state.hpp"  
#include "states/shop_state.hpp"
#include "states/tutorial_state.hpp"
#include "services/resource_manager.hpp"
#include "core/game.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>

MenuState::MenuState() {
    ResourceManager::instance().playMusic(Config::BG_MUSIC);
    const auto& bgTex = ResourceManager::instance().getTexture(Config::MENU_BG_TEXTURE);
    m_bgMenu_ = std::make_unique<sf::Sprite>(bgTex);
    m_bgMenu_->setPosition({0.f, 0.f});

    // 1. Thêm textureId vào để biết nút nào dùng ảnh nào
    struct ButtonConfig {
        std::string textureId;
        std::string label;
        ButtonAction action;
    };

    // 2. Thiết lập danh sách nút
    std::vector<ButtonConfig> configs = {
        {Config::BTN_START_TEXTURE, "START", ButtonAction::Start},           
        {Config::BTN_BLANK_TEXTURE, "CONTINUE", ButtonAction::Continue},
        {Config::BTN_BLANK_TEXTURE, "SHOP", ButtonAction::Shop},
        {Config::BTN_BLANK_TEXTURE, "SCORE BOARD", ButtonAction::Scoreboard},
        {Config::BTN_BLANK_TEXTURE, "TUTORIAL", ButtonAction::Tutorial},
        {Config::BTN_BLANK_TEXTURE, "OPTION", ButtonAction::Option},
        {Config::BTN_BLANK_TEXTURE, "EXIT", ButtonAction::Exit}
    };

    // Lấy Font chữ
    const sf::Font& mainFont = ResourceManager::instance().getFont(Config::MAIN_FONT);

    float startX = 800.f;
    float startY = 160.f;
    float stepY = 80.f;

    // 3. Vòng lặp khởi tạo
    for (size_t i = 0; i < configs.size(); ++i) {
        UIButton btn(mainFont); 
        btn.action = configs[i].action;
        
        // --- XỬ LÝ BACKGROUND NÚT (Load đúng texture tương ứng) ---
        const sf::Texture& tex = ResourceManager::instance().getTexture(configs[i].textureId);
        btn.sprite = std::make_unique<sf::Sprite>(tex);
        centerOrigin(*(btn.sprite));
        btn.sprite->setPosition({startX, startY + (i * stepY)});

        // --- XỬ LÝ CHỮ TRÊN NÚT ---
        if (!configs[i].label.empty()) {
            btn.text.setString(configs[i].label);
            btn.text.setCharacterSize(30);
            btn.text.setFillColor(sf::Color::White);
            
            // Căn giữa chữ
            sf::FloatRect textBounds = btn.text.getLocalBounds();
            btn.text.setOrigin({
                textBounds.position.x + textBounds.size.x / 2.f,
                textBounds.position.y + textBounds.size.y / 2.f
            });
            
            // Ép chữ nằm đè đúng lên vị trí của nút
            btn.text.setPosition({startX, startY + (i * stepY)});
        }

        m_buttons_.push_back(std::move(btn));
    }
}

void MenuState::processEvents(Game* game, const std::optional<sf::Event>& event) {
    if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = game->getWindow().mapPixelToCoords(mouseEvent->position);
            
            for (const auto& btn : m_buttons_) {
                if (isClicked(*(btn.sprite), mousePos)) {
                    switch (btn.action) {
                        case ButtonAction::Start:
                            game->pushState(new ModeState());
                            break;
                        case ButtonAction::Continue:
                            game->pushState(new LoadState());
                            break;
                        case ButtonAction::Shop:
                            game->pushState(new ShopState());
                            break;
                        case ButtonAction::Exit:
                            game->getWindow().close();
                            break;
                        case ButtonAction::Option:
                            game->pushState(new SettingState());
                            break;
                        case ButtonAction::Scoreboard:
                            game->pushState(new ScoreboardState());
                            break;
                        case ButtonAction::Tutorial:
                            game->pushState(new TutorialState());
                            break;
                        default:
                            break;
                    }
                    break;
                }
            }
        }
    }
}

void MenuState::update(Game* game, float dt) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(game->getWindow());
    sf::Vector2f mousePos = game->getWindow().mapPixelToCoords(pixelPos);

    static float time = 0.f;
    time += dt;

    // Vòng lặp update cực kì gọn gàng
    for (auto& btn : m_buttons_) {
        
        if (isMouseHovering(*(btn.sprite), game->getWindow())) {
            
            // --- TRẠNG THÁI HOVER (Kinh dị) ---
            float pulse = 1.05f + std::sin(time * 15.f) * 0.05f; 
            float jitter = std::cos(time * 30.f) * 1.5f;         
            
            btn.sprite->setScale({pulse, pulse});
            btn.sprite->setRotation(sf::degrees(jitter));
            btn.text.setScale({pulse, pulse});
            btn.text.setRotation(sf::degrees(jitter));
            
            std::uint8_t alpha = 180 + static_cast<std::uint8_t>(std::abs(std::sin(time * 10.f)) * 75);
            btn.sprite->setColor(sf::Color(255, 150, 150, alpha));
            btn.text.setFillColor(sf::Color(255, 20, 20));

        } else {
            // --- TRẠNG THÁI BÌNH THƯỜNG ---
            btn.sprite->setScale({1.0f, 1.0f});
            btn.sprite->setRotation(sf::degrees(0.f));
            btn.sprite->setColor(sf::Color::White);

            btn.text.setScale({1.0f, 1.0f});
            btn.text.setRotation(sf::degrees(0.f));
            btn.text.setFillColor(sf::Color::White);
        }
    }
}

void MenuState::render(sf::RenderWindow& window) {
    // Vẽ nút "New Game", "Load Game" ra màn hình
    window.clear(sf::Color::Black);
    window.draw(*m_bgMenu_);
    for (auto& btn : m_buttons_) {
        window.draw(*(btn.sprite));
        window.draw(btn.text);
    }window.draw(*m_bgMenu_);
    for (auto& btn : m_buttons_) {
        window.draw(*(btn.sprite));
        window.draw(btn.text);
    }

}