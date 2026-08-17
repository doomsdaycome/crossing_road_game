#include "states/scoreboard_state.hpp"
#include "states/menu_state.hpp"
#include "core/game.hpp"
#include "core/config.hpp"
#include "services/resource_manager.hpp"
#include "utils/utils.hpp"
#include "gameplay/score_manager.hpp" 
#include <iostream>

ScoreboardState::ScoreboardState() {
    auto& rm = ResourceManager::instance();
    const sf::Font& mainFont = rm.getFont(Config::MAIN_FONT);

    // 1. SETUP BACKGROUND (Dùng đúng OPTION_BG_TEXTURE)
    const sf::Texture& bgTex = rm.getTexture(Config::OPTION_BG_TEXTURE);
    m_bgSprite_ = std::make_unique<sf::Sprite>(bgTex);
    // Tùy chọn: Nếu ảnh của ông nhỏ/to hơn màn hình thì uncomment 3 dòng dưới để fit vừa cửa sổ
    // float scaleX = Config::WINDOW_WIDTH / m_bgSprite_->getGlobalBounds().size.x;
    // float scaleY = Config::WINDOW_HEIGHT / m_bgSprite_->getGlobalBounds().size.y;
    // m_bgSprite_->setScale(sf::Vector2f{scaleX, scaleY});

    // 2. NHỜ SCORE MANAGER LÀM VIỆC NẶNG
    ScoreManager tempScoreManager;
    tempScoreManager.loadHighScore();
    int bestScore = tempScoreManager.getBestScore();
    int totalGold = tempScoreManager.getTotalCoins();

    // 3. SETUP TIÊU ĐỀ (Chuẩn SFML 3.x truyền sf::Vector2f)
    m_titleText_ = std::make_unique<sf::Text>(mainFont);
    m_titleText_->setString("HALL OF FAME");
    m_titleText_->setCharacterSize(70);
    m_titleText_->setFillColor(sf::Color(100, 30, 20)); 
    m_titleText_->setOutlineColor(sf::Color::White);
    m_titleText_->setOutlineThickness(2.f);
    centerOrigin(*m_titleText_);
    m_titleText_->setPosition(sf::Vector2f{Config::WINDOW_WIDTH / 2.f, 150.f});

    // 4. SETUP KỶ LỤC
    m_bestScoreText_ = std::make_unique<sf::Text>(mainFont);
    m_bestScoreText_->setString("BEST SCORE: " + std::to_string(bestScore));
    m_bestScoreText_->setCharacterSize(40);
    m_bestScoreText_->setFillColor(sf::Color(60, 40, 20));
    centerOrigin(*m_bestScoreText_);
    m_bestScoreText_->setPosition(sf::Vector2f{Config::WINDOW_WIDTH / 2.f, 300.f});

    // 5. SETUP TỔNG VÀNG
    m_totalGoldText_ = std::make_unique<sf::Text>(mainFont);
    m_totalGoldText_->setString("TOTAL COINS: " + std::to_string(totalGold));
    m_totalGoldText_->setCharacterSize(40);
    m_totalGoldText_->setFillColor(sf::Color(60, 40, 20));
    centerOrigin(*m_totalGoldText_);
    m_totalGoldText_->setPosition(sf::Vector2f{Config::WINDOW_WIDTH / 2.f, 400.f});

    // 6. SETUP RANK TÙY CHỌN (Giữ lại cho đỡ trống, xóa thì bỏ đoạn này đi)
    m_rankText_ = std::make_unique<sf::Text>(mainFont);
    std::string rankStr = "RANK: C (ROOKIE)";
    sf::Color rankColor = sf::Color(100, 100, 100);
    
    if (bestScore >= 500) { rankStr = "RANK: S (LEGEND)"; rankColor = sf::Color(255, 50, 50); }
    else if (bestScore >= 300) { rankStr = "RANK: A (PRO)"; rankColor = sf::Color(255, 150, 0); }
    else if (bestScore >= 100) { rankStr = "RANK: B (VETERAN)"; rankColor = sf::Color(50, 255, 50); }

    m_rankText_->setString(rankStr);
    m_rankText_->setCharacterSize(45);
    m_rankText_->setFillColor(rankColor);
    centerOrigin(*m_rankText_);
    m_rankText_->setPosition(sf::Vector2f{Config::WINDOW_WIDTH / 2.f, 500.f});

    // 7. SETUP NÚT BACK 
    const sf::Texture& btnTex = rm.getTexture(Config::BTN_BLANK_TEXTURE);
    m_backBtnSprite_ = std::make_unique<sf::Sprite>(btnTex);
    centerOrigin(*m_backBtnSprite_);
    m_backBtnSprite_->setPosition(sf::Vector2f{Config::WINDOW_WIDTH / 2.f, 600.f});

    m_backBtnText_ = std::make_unique<sf::Text>(mainFont, "BACK", 25);
    m_backBtnText_->setFillColor(sf::Color(60, 40, 20));
    centerOrigin(*m_backBtnText_);
    m_backBtnText_->setPosition(m_backBtnSprite_->getPosition());
}

void ScoreboardState::update(Game* game, float dt) {
    static float time = 0.f;
    time += dt;

    if (isMouseHovering(*m_backBtnSprite_, game->getWindow())) {
        float pulse = 1.0f + std::sin(time * 15.f) * 0.05f; 
        m_backBtnSprite_->setScale({pulse, pulse});
        m_backBtnText_->setScale({pulse, pulse});
        m_backBtnSprite_->setColor(sf::Color(255, 150, 150));
    } else {
        m_backBtnSprite_->setScale({1.0f, 1.0f});
        m_backBtnText_->setScale({1.0f, 1.0f});
        m_backBtnSprite_->setColor(sf::Color::White);
    }
}

void ScoreboardState::render(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    
    window.draw(*m_bgSprite_);
    window.draw(*m_titleText_);
    window.draw(*m_bestScoreText_);
    window.draw(*m_totalGoldText_);
    window.draw(*m_rankText_); 
    
    window.draw(*m_backBtnSprite_);
    window.draw(*m_backBtnText_);
}

void ScoreboardState::processEvents(Game* game, const std::optional<sf::Event>& event) {
    if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f worldPos = game->getWindow().mapPixelToCoords(mouseEvent->position);
            
            // Bấm vào nút Back thì Pop State này ra để quay lại Menu
            if (isClicked(*m_backBtnSprite_, worldPos)) {
                game->popState(); 
            }
        }
    }
}