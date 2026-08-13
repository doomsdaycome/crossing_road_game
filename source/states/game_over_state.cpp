#include "states/game_over_state.hpp"
#include "states/menu_state.hpp"
#include "states/play_state.hpp"
#include "services/resource_manager.hpp"
#include "core/game.hpp"
#include "core/config.hpp"
#include <cmath>
#include <algorithm>

// SỬA LẠI CONSTRUCTOR: Nhận thêm cờ isVictory và số vàng ăn được
GameOverState::GameOverState(int score, GameMode mode, int level, bool isVictory, int earnedGold, int bestScore, int totalCoins) 
    : m_finalScore_(score), m_mode_(mode), m_level_(level), m_isVictory_(isVictory), 
      m_earnedGold_(earnedGold), m_displayScore_(0.f), m_hasCollided_(false), m_shakeTimer_(0.f) {
    
    auto& rm = ResourceManager::instance();
    const sf::Font& mainFont = rm.getFont(Config::MAIN_FONT);
    const sf::Texture& rectTex = rm.getTexture(Config::BTN_BLANK_TEXTURE);

    // ==========================================
    // 1. LẤY DỮ LIỆU TỪ Ổ CỨNG (Vàng tổng & Kỷ lục)
    // ==========================================
    // TODO: Gắn class đọc file profile của ông vào đây
    m_oldGold_ = totalCoins - m_earnedGold_; 
    
    m_highScore_ = bestScore; 
    
    m_displayGold_ = static_cast<float>(m_oldGold_);

    // ==========================================
    // 2. KHỞI TẠO UI BỊ GIẤU NGOÀI MÀN HÌNH
    // ==========================================
    m_bgOverlay_.setSize({Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT});
    m_bgOverlay_.setFillColor(sf::Color(15, 10, 10, 200)); 

    m_targetX_ = Config::WINDOW_WIDTH / 2.f;
    m_leftX_ = -600.f; 
    m_rightX_ = Config::WINDOW_WIDTH + 600.f; 

    // --- Tiêu đề (Biến hình theo Victory/Game Over) ---
    m_titleText_ = std::make_unique<sf::Text>(mainFont);
    m_titleText_->setString(m_isVictory_ ? "VICTORY" : "GAME OVER");
    m_titleText_->setCharacterSize(90);
    // Thắng thì Vàng óng, Thua thì Đỏ thẫm
    m_titleText_->setFillColor(m_isVictory_ ? sf::Color(255, 215, 0) : sf::Color(220, 20, 20));
    m_titleText_->setOutlineColor(sf::Color::White);
    m_titleText_->setOutlineThickness(3.f);
    centerOrigin(*m_titleText_);

    // --- Điểm số (Khởi tạo bằng 0 để lát nhảy số) ---
    m_scoreText_ = std::make_unique<sf::Text>(mainFont);
    m_scoreText_->setString("SCORE: 0");
    m_scoreText_->setCharacterSize(45);
    m_scoreText_->setFillColor(sf::Color::White);
    centerOrigin(*m_scoreText_);

    // --- Kỷ lục (Chỉ xuất hiện ở Endless) ---
    if (m_mode_ == GameMode::ENDLESS) {
        m_highScoreText_ = std::make_unique<sf::Text>(mainFont);
        m_highScoreText_->setString("BEST: " + std::to_string(m_highScore_));
        m_highScoreText_->setCharacterSize(30);
        m_highScoreText_->setFillColor(sf::Color(255, 150, 50));
        centerOrigin(*m_highScoreText_);
    }

    // --- Tổng vàng ---
    m_goldText_ = std::make_unique<sf::Text>(mainFont);
    m_goldText_->setString("GOLD: " + std::to_string(m_oldGold_));
    m_goldText_->setCharacterSize(45);
    m_goldText_->setFillColor(sf::Color::Yellow);
    m_goldText_->setOutlineColor(sf::Color(100, 80, 20));
    m_goldText_->setOutlineThickness(2.f);
    centerOrigin(*m_goldText_);

    // --- Các nút bấm ---
    UIButton retryBtn(mainFont);
    retryBtn.sprite = std::make_unique<sf::Sprite>(rectTex);
    centerOrigin(*(retryBtn.sprite));
    // Đổi chữ nút nếu Thắng màn Classic
    retryBtn.text.setString((m_isVictory_ && m_mode_ == GameMode::CLASSIC) ? "NEXT LEVEL" : "PLAY AGAIN");
    retryBtn.text.setCharacterSize(25);
    retryBtn.text.setFillColor(sf::Color(60, 40, 20));
    centerOrigin(retryBtn.text);
    retryBtn.action = ButtonAction::Restart; 
    m_buttons_.push_back(std::move(retryBtn));

    UIButton homeBtn(mainFont);
    homeBtn.sprite = std::make_unique<sf::Sprite>(rectTex);
    centerOrigin(*(homeBtn.sprite));
    homeBtn.text.setString("HOME");
    homeBtn.text.setCharacterSize(25);
    homeBtn.text.setFillColor(sf::Color(60, 40, 20));
    centerOrigin(homeBtn.text);
    homeBtn.action = ButtonAction::Home;
    m_buttons_.push_back(std::move(homeBtn));
}

void GameOverState::update(Game* game, float dt) {
    // 1. TRƯỢT MÀN HÌNH THEO LERP
    float speed = 12.f; 
    m_leftX_ += (m_targetX_ - m_leftX_) * speed * dt;
    m_rightX_ += (m_targetX_ - m_rightX_) * speed * dt;

    if (!m_hasCollided_ && std::abs(m_targetX_ - m_leftX_) < 5.f) {
        m_hasCollided_ = true;
        m_shakeTimer_ = 0.2f; 
    }

    if (m_shakeTimer_ > 0.f) {
        m_shakeTimer_ -= dt;
    }

    // ==========================================
    // 2. HIỆU ỨNG NHẢY SỐ (Chỉ bắt đầu khi m_hasCollided_ == true)
    // ==========================================
    if (m_hasCollided_) {
        // Đếm Điểm
        float scoreSpeed = std::max(100.f, m_finalScore_ * 1.5f); // Tốc độ đếm linh hoạt theo tổng điểm
        if (m_displayScore_ < m_finalScore_) {
            m_displayScore_ += scoreSpeed * dt;
            if (m_displayScore_ > m_finalScore_) m_displayScore_ = static_cast<float>(m_finalScore_);
            
            m_scoreText_->setString("SCORE: " + std::to_string(static_cast<int>(m_displayScore_)));
            centerOrigin(*m_scoreText_); // Gọi lại centerOrigin để căn giữa lại khi số dài ra
        }

        // Đếm Vàng
        float targetGold = static_cast<float>(m_oldGold_ + m_earnedGold_);
        float goldSpeed = std::max(50.f, m_earnedGold_ * 2.0f); 
        if (m_displayGold_ < targetGold) {
            m_displayGold_ += goldSpeed * dt;
            if (m_displayGold_ > targetGold) m_displayGold_ = targetGold;

            m_goldText_->setString("GOLD: " + std::to_string(static_cast<int>(m_displayGold_)));
            centerOrigin(*m_goldText_);
        }
    }

    // ==========================================
    // 3. SET LẠI TỌA ĐỘ THEO BIẾN TRƯỢT m_leftX_ VÀ m_rightX_
    // ==========================================
    m_titleText_->setPosition({m_leftX_, 150.f}); // Đẩy tiêu đề cao lên tí lấy chỗ trống
    
    // Sắp xếp các text bên m_rightX_ thành 1 cột dọc
    float currentY = 250.f;
    m_scoreText_->setPosition({m_rightX_, currentY});
    currentY += 50.f;
    
    if (m_highScoreText_) {
        m_highScoreText_->setPosition({m_rightX_, currentY});
        currentY += 40.f;
    }

    m_goldText_->setPosition({m_rightX_, currentY});
    
    // Sắp xếp nút bấm
    float startBtnY = currentY + 80.f;
    float gapY = 85.f;
    
    for (size_t i = 0; i < m_buttons_.size(); ++i) {
        m_buttons_[i].sprite->setPosition({m_rightX_, startBtnY + (i * gapY)});
        m_buttons_[i].text.setPosition({m_rightX_, startBtnY + (i * gapY)});
        
        if (m_hasCollided_) {
            if (isMouseHovering(*(m_buttons_[i].sprite), game->getWindow())) {
                m_buttons_[i].sprite->setScale({1.1f, 1.1f});
                m_buttons_[i].text.setScale({1.1f, 1.1f});
            } else {
                m_buttons_[i].sprite->setScale({1.0f, 1.0f});
                m_buttons_[i].text.setScale({1.0f, 1.0f});
            }
        }
    }
}

void GameOverState::render(sf::RenderWindow& window) {
    sf::View view = window.getDefaultView();
    if (m_shakeTimer_ > 0.f) {
        float offsetX = (std::rand() % 12 - 6) * 1.0f;
        float offsetY = (std::rand() % 12 - 6) * 1.0f;
        view.move({offsetX, offsetY});
    }
    window.setView(view);

    window.draw(m_bgOverlay_);
    window.draw(*m_titleText_);
    window.draw(*m_scoreText_);
    window.draw(*m_goldText_);
    
    // Chỉ vẽ HighScore nếu nó có tồn tại (Tức là đang ở Endless)
    if (m_highScoreText_) {
        window.draw(*m_highScoreText_);
    }

    for (const auto& btn : m_buttons_) {
        window.draw(*(btn.sprite));
        window.draw(btn.text);
    }

    window.setView(window.getDefaultView());
}

void GameOverState::processEvents(Game* game, const std::optional<sf::Event>& event) {
    if (!m_hasCollided_) return;

    if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f worldPos = game->getWindow().mapPixelToCoords(mouseEvent->position);
            
            for (const auto& btn : m_buttons_) {
                if (isClicked(*(btn.sprite), worldPos)) {
                    
                    if (btn.action == ButtonAction::Restart) {
                        // Nạp lại màn chơi mới! Nếu Thắng Classic thì tăng Level lên 1
                        int targetLevel = (m_isVictory_ && m_mode_ == GameMode::CLASSIC) ? (m_level_ + 1) : m_level_;
                        game->changeState(new PlayingState(m_mode_, targetLevel), true);
                    } 
                    else if (btn.action == ButtonAction::Home) {
                        game->changeState(new MenuState(), true);
                    }
                    return;
                }
            }
        }
    }
}