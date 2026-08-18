#include "states/play_state.hpp"
#include "states/setting_state.hpp"
#include "states/game_over_state.hpp"
#include "core/game.hpp"
#include "data/level_data.hpp"
#include "core/config.hpp"
#include "services/level_repository.hpp"
#include "services/save_game_repository.hpp"
#include "services/resource_manager.hpp"
#include "gameplay/collision_system.hpp"
#include <iostream>

// ==========================================
// 1. CONSTRUCTOR: NEW GAME
// ==========================================
PlayingState::PlayingState(GameMode gameMode, int level) {
    m_mode_ = gameMode;
    m_currentLevel_ = level;
    
    // --- Khởi tạo UI và Hiệu ứng ---
    const sf::Texture& overlayTex = ResourceManager::instance().getTexture(Config::TOP_OVERLAY_TEXTURE);
    m_topOverlaySprite_ = std::make_unique<sf::Sprite>(overlayTex);

    const sf::Font& mainFont = ResourceManager::instance().getFont(Config::MAIN_FONT);
    m_fightEffect_ = std::make_unique<sf::Text>(mainFont);
    m_fightEffect_->setString("BAM!");
    m_fightEffect_->setCharacterSize(40);
    m_fightEffect_->setFillColor(sf::Color::Red);
    m_fightEffect_->setOutlineColor(sf::Color::Yellow);
    m_fightEffect_->setOutlineThickness(3.f);
    centerOrigin(*m_fightEffect_);

    m_hudText_ = std::make_unique<sf::Text>(mainFont);
    m_hudText_->setCharacterSize(24);
    m_hudText_->setFillColor(sf::Color::White);
    m_hudText_->setOutlineColor(sf::Color::Black);
    m_hudText_->setOutlineThickness(2.f);
    m_hudText_->setPosition({20.f, 20.f}); // Đặt ở góc trái trên

    // --- Khởi tạo Logic theo Chế độ ---
    if (m_mode_ == GameMode::CLASSIC) {
        std::string filepath = Config::LEVEL_PATH_PREFIX + std::to_string(m_currentLevel_) + Config::LEVEL_PATH_SUFFIX;
        m_currentLevelData_ = LevelRepository::loadLevel(filepath);
        m_laneManager_.buildFromSaveData(m_currentLevelData_.lanes);
        m_laneManager_.initBackground(0.f);

        // Setup Cửa Chiến Thắng (Chuẩn SFML 3.x)
        const sf::Texture& finishTex = ResourceManager::instance().getTexture(Config::FINISH_LINE_TEXTURE);
        m_finishLineSprite_ = std::make_unique<sf::Sprite>(finishTex);
        m_finishLineSprite_->setTextureRect(sf::IntRect({0, 0}, {1024, 150}));
        
        if (!m_currentLevelData_.lanes.empty()) {
            float topOfMapY = m_currentLevelData_.lanes.back().yPosition;
            float spriteHeight = m_finishLineSprite_->getGlobalBounds().size.y;
            m_finishLineY_ = topOfMapY - spriteHeight; 
            m_finishLineSprite_->setPosition({0.f, m_finishLineY_ - 35.f});
        }
    }
    else if (m_mode_ == GameMode::ENDLESS) {
        m_laneManager_.initEndless();
        m_laneManager_.initBackground(0.f);
    }
    
    // Nạp Kỷ lục & Lưu mốc Y ban đầu cho cả 2 mode
    m_score_.loadHighScore();
    m_player_.setSkin(m_score_.getEquippedSkin());
    m_minGridY_ = m_player_.getGridY();

    initHUD();
    ResourceManager::instance().playMusic(Config::BGM_PLAYING);
}

// ==========================================
// 2. CONSTRUCTOR: LOAD GAME (Khôi phục trạng thái)
// ==========================================
PlayingState::PlayingState(const GameSnapshot& snapshot) {
    const sf::Texture& overlayTex = ResourceManager::instance().getTexture(Config::TOP_OVERLAY_TEXTURE);
    m_topOverlaySprite_ = std::make_unique<sf::Sprite>(overlayTex);

    const sf::Font& mainFont = ResourceManager::instance().getFont(Config::MAIN_FONT);
    m_fightEffect_ = std::make_unique<sf::Text>(mainFont);
    m_fightEffect_->setString("BAM!");
    m_fightEffect_->setCharacterSize(40);
    m_fightEffect_->setFillColor(sf::Color::Red);
    m_fightEffect_->setOutlineColor(sf::Color::Yellow);
    m_fightEffect_->setOutlineThickness(3.f);
    centerOrigin(*m_fightEffect_);

    m_hudText_ = std::make_unique<sf::Text>(mainFont);
    m_hudText_->setCharacterSize(24);
    m_hudText_->setFillColor(sf::Color::White);
    m_hudText_->setOutlineColor(sf::Color::Black);
    m_hudText_->setOutlineThickness(2.f);
    m_hudText_->setPosition({20.f, 20.f});

    m_mode_ = GameMode::ENDLESS;
    m_currentLevel_ = 0;

    // Nạp lại toàn bộ Map và Nhân vật
    m_laneManager_.buildFromSaveData(snapshot.lanes);
    m_laneManager_.initBackground(snapshot.playerGridY * Config::TILE_SIZE);
    m_player_.loadState(snapshot.playerGridX, snapshot.playerGridY);
    m_score_.loadScore(snapshot.score);

    m_camera_.setCenterY(snapshot.cameraY);

    // --- CƠ CHẾ BẢO VỆ NGƯỜI CHƠI KHI LOAD GAME ---
    // Đảm bảo nhân vật không bị nằm sát mép dưới màn hình (cách ít nhất 250px)
    float playerY = m_player_.getGridY() * Config::TILE_SIZE;
    float cameraBottom = m_camera_.getView().getCenter().y + (Config::WINDOW_HEIGHT / 2.f);
    
    if (cameraBottom - playerY < 250.f) {
        float newCameraBottom = playerY + 250.f;
        m_camera_.setCenterY(newCameraBottom - (Config::WINDOW_HEIGHT / 2.f));
    }
    m_coinsCollectedInRun_ = snapshot.coinsInRun;

    m_score_.loadHighScore();
    m_player_.setSkin(m_score_.getEquippedSkin());
    m_minGridY_ = m_player_.getGridY();

    initHUD();
    ResourceManager::instance().playMusic(Config::BGM_PLAYING);
}

// ==========================================
// 3. XỬ LÝ SỰ KIỆN PHÍM BẤM
// ==========================================
void PlayingState::processEvents(Game* game, const std::optional<sf::Event>& event) {
    if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
        
        // Bấm ESC để Pause Game
        if (keyPress->code == sf::Keyboard::Key::Escape) {
            if (m_mode_ == GameMode::ENDLESS) {
                // Gom Data và Ghi Kỷ lục trước khi Pause
                GameSnapshot snapshot;
                snapshot.score = m_score_.getScore();
                snapshot.playerGridX = m_player_.getGridX();
                snapshot.playerGridY = m_player_.getGridY();
                snapshot.lanes = m_laneManager_.exportSaveData();
                snapshot.cameraY = m_camera_.getView().getCenter().y; 
                snapshot.coinsInRun = m_coinsCollectedInRun_;
                syncBuffsToScore();
                m_score_.saveHighScore(); 

                game->pushState(new SettingState(true, snapshot));
            } else {
                game->pushState(new SettingState(false)); 
            }
            return;
        }

        // Bất kỳ phím di chuyển nào cũng làm Game Started = true
        if (keyPress->code == sf::Keyboard::Key::A || keyPress->code == sf::Keyboard::Key::Left ||
            keyPress->code == sf::Keyboard::Key::W || keyPress->code == sf::Keyboard::Key::Up ||
            keyPress->code == sf::Keyboard::Key::D || keyPress->code == sf::Keyboard::Key::Right ||
            keyPress->code == sf::Keyboard::Key::S || keyPress->code == sf::Keyboard::Key::Down) 
        {
            if (!isGameStarted_) isGameStarted_ = true;
        }

        // Kích hoạt Buff
        if (keyPress->code == sf::Keyboard::Key::Num1) {
            m_player_.getBuffManager().consumeMagnetItem(10.f); // Magnet 10s
        } else if (keyPress->code == sf::Keyboard::Key::Num2) {
            m_player_.getBuffManager().consumeInvisibilityItem(5.f); // Invisibility 5s
        } else if (keyPress->code == sf::Keyboard::Key::Space) {
            if (m_player_.getBuffManager().consumeHolyNova()) {
                m_laneManager_.clearAllMonsters();
                m_flashAlpha_ = 255.f;
            }
        }
    }
    m_player_.processEvents(event, [this](float targetY) {
        return m_laneManager_.isChasmLane(targetY);
    });
}

// ==========================================
// 4. CẬP NHẬT LOGIC GAME (Mỗi Frame)
// ==========================================
void PlayingState::update(Game* game, float dt) {
    // --- Khóa update nếu đang chạy Cutscene ---
    if (m_introCutscene_.isBusy()) {
        m_introCutscene_.update(dt);
        return; 
    }

    // ==========================================
    // KHỐI LOGIC 1: ĐÃ CHIẾN THẮNG (Chỉ cho Classic)
    // ==========================================
    if (m_isVictory_) {
        m_victoryTimer_ += dt;
        
        if (m_victoryTimer_ > 1.5f) { 
            // Gom vàng và Lưu file Level
            m_currentLevelData_.collectedCoins += m_coinsCollectedInRun_;
            if (m_currentLevelData_.collectedCoins > m_currentLevelData_.totalCoins) {
                m_currentLevelData_.collectedCoins = m_currentLevelData_.totalCoins;
            }
            m_currentLevelData_.lanes = m_laneManager_.exportSaveData();
            
            std::string filepath = Config::LEVEL_PATH_PREFIX + std::to_string(m_currentLevel_) + Config::LEVEL_PATH_SUFFIX;
            LevelRepository::saveLevel(filepath, m_currentLevelData_);
            
            // Lưu tổng vàng toàn game
            m_score_.addTotalCoins(m_coinsCollectedInRun_);
            syncBuffsToScore();
            m_score_.saveHighScore();

            // Chuyển State (Cập nhật đủ 7 tham số)
            game->changeState(new GameOverState(
                m_score_.getScore(), m_mode_, m_currentLevel_, true, 
                m_coinsCollectedInRun_, m_score_.getBestScore(), m_score_.getTotalCoins()
            ));
        }
        return; 
    }

    // ==========================================
    // KHỐI LOGIC 2: ĐÃ THUA (Chết do quái/camera)
    // ==========================================
    if (m_isCameraDeath_ || m_isMonsterDeath_) {
        m_shakeTimer_ -= dt;
        
        if (m_isMonsterDeath_) {
            float scale = 1.0f + std::abs(std::sin(m_shakeTimer_ * 20.f)) * 0.5f;
            m_fightEffect_->setScale({scale, scale});
        }

        if (m_shakeTimer_ <= 0.f) {
            m_score_.addTotalCoins(m_coinsCollectedInRun_);
            syncBuffsToScore();
            m_score_.saveHighScore();
            
            game->pushState(new GameOverState(
                m_score_.getScore(), m_mode_, m_currentLevel_, false, 
                m_coinsCollectedInRun_, m_score_.getBestScore(), m_score_.getTotalCoins()
            ));
        }
        return; 
    }

    // ==========================================
    // KHỐI LOGIC 3: GAMEPLAY CHÍNH
    // ==========================================
    m_giantEye.update(dt);

    bool isRedLight = m_giantEye.isRedLightActive();

    float slowMoFactor = isRedLight ? 0.4f : 1.0f;
    float scaledDt = dt * slowMoFactor;

    m_player_.update(scaledDt);

    // Cập nhật hiệu ứng Flash (Holy Nova)
    if (m_flashAlpha_ > 0.f) {
        m_flashAlpha_ -= 500.f * dt;
        if (m_flashAlpha_ < 0.f) m_flashAlpha_ = 0.f;
        m_whiteFlashRect_.setFillColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(m_flashAlpha_)));
        
        m_whiteFlashRect_.setSize({static_cast<float>(Config::VIEW_WIDTH), static_cast<float>(Config::VIEW_HEIGHT)});
        m_whiteFlashRect_.setPosition({
            m_camera_.getView().getCenter().x - Config::VIEW_WIDTH / 2.f,
            m_camera_.getView().getCenter().y - Config::VIEW_HEIGHT / 2.f
        });
    }

    // XUỐNG DÒNG VÀNG: Mở khóa logic nhặt vàng cho CẢ 2 CHẾ ĐỘ
    m_coinsCollectedInRun_ += CollisionSystem::checkAndCollectCoins(m_player_, m_laneManager_);

    // PHÂN NHÁNH A: ENDLESS MODE
    if (m_mode_ == GameMode::ENDLESS) {
        int currentY = m_player_.getGridY();
        if (currentY < m_minGridY_) {
            int steps = m_minGridY_ - currentY;
            m_score_.addPoints(steps * 10); 
            m_minGridY_ = currentY; 
        }

        if (m_hudText_) {
            m_hudText_->setString("SCORE: " + std::to_string(m_score_.getScore()) + 
                                  "    BEST: " + std::to_string(m_score_.getBestScore()) +
                                  "    COINS: " + std::to_string(m_coinsCollectedInRun_));
        }
    }
    // PHÂN NHÁNH B: CLASSIC MODE
    else if (m_mode_ == GameMode::CLASSIC) {
        if (m_hudText_) {
            m_hudText_->setString("LEVEL: " + std::to_string(m_currentLevel_) + 
                                  "    COINS: " + std::to_string(m_coinsCollectedInRun_));
        }

        // Check chạm đích
        float playerWorldY = static_cast<float>(m_player_.getGridY() * Config::TILE_SIZE);
        if (playerWorldY <= m_finishLineY_ + Config::TILE_SIZE) {
            m_isVictory_ = true;
        }
    }
    
    // --- HUD Cập nhật số lượng Buff ---
    int counts[4] = {
        m_player_.getBuffManager().getMagnetCount(),
        m_player_.getBuffManager().getShieldCount(),
        m_player_.getBuffManager().getInvisCount(),
        m_player_.getBuffManager().getNovaCount()
    };
    for (int i = 0; i < 4; ++i) {
        m_buffCounts_[i].setString("x" + std::to_string(counts[i]));
        if (counts[i] <= 0) {
            m_buffIcons_[i].setFillColor(sf::Color(100, 100, 100, 150));
            m_buffCounts_[i].setFillColor(sf::Color(150, 150, 150, 150));
            m_buffKeys_[i].setFillColor(sf::Color(150, 150, 150, 150));
        } else {
            m_buffIcons_[i].setFillColor(sf::Color::White); // Không tô màu đè lên ảnh
            m_buffCounts_[i].setFillColor(sf::Color::White);
            m_buffKeys_[i].setFillColor(sf::Color::White);
        }
    }

    // --- Cập nhật Camera và Làn đường ---
    if (isGameStarted_) {
        bool shouldScrollCamera = true;
        
        // Khóa camera nếu tới đích (Classic)
        if (m_mode_ == GameMode::CLASSIC && m_camera_.getTopEdge() <= m_finishLineY_ - 50.f) {
            shouldScrollCamera = false; 
        }

        if (shouldScrollCamera) {
            // 1. Lấy tốc độ cuộn gốc của màn chơi
            float camSpeed = m_laneManager_.getGlobalSpeedMultiplier();
            
            // 2. Tính khoảng cách từ nhân vật đến mép trên của Camera
            float playerWorldY = static_cast<float>(m_player_.getGridY() * Config::TILE_SIZE);
            float distanceToTop = playerWorldY - m_camera_.getTopEdge();
            
            // 3. Đặt ngưỡng kích hoạt (Ví dụ: 1/3 chiều cao màn hình)
            // Nếu màn hình của ông cao 768px, threshold sẽ là 256px.
            // Ông có thể thay Config::WINDOW_HEIGHT bằng Config::VIEW_HEIGHT tùy thuộc ông đang xài biến nào.
            float threshold = Config::WINDOW_HEIGHT / 3.0f; 
            
            // 4. Nếu nhân vật lọt vào vùng "Nguy hiểm phía trên"
            if (distanceToTop < threshold) {
                // Tăng tốc camera lên gấp 2.5 lần để đuổi theo nhân vật!
                // Khi nhân vật lùi về giữa màn hình (distanceToTop > threshold), 
                // tốc độ sẽ tự động reset về bình thường vì camSpeed không bị nhân nữa.
                camSpeed *= 2.5f; 
            }

            m_camera_.update(scaledDt, camSpeed);
        }
    }

    bool isEndless = (m_mode_ == GameMode::ENDLESS);
    m_laneManager_.update(dt, isEndless, m_camera_.getTopEdge(), m_camera_.getBottomEdge(), isRedLight);

    // --- Kiểm tra Va chạm Gây chết ---
    if (CollisionSystem::checkPlayerVsMonsters(m_player_, m_laneManager_)) {
        m_player_.die();
        m_isMonsterDeath_ = true; 
        m_shakeTimer_ = 0.5f;     
        
        float pX = static_cast<float>(m_player_.getGridX() * Config::TILE_SIZE + Config::TILE_SIZE / 2);
        float pY = static_cast<float>(m_player_.getGridY() * Config::TILE_SIZE + Config::TILE_SIZE / 2);
        m_fightEffect_->setPosition({pX, pY});
        return;
    }

    float playerWorldY = static_cast<float>(m_player_.getGridY() * Config::TILE_SIZE);
    if (playerWorldY > m_camera_.getBottomEdge() + Config::TILE_SIZE) {
        m_player_.die();
        m_isCameraDeath_ = true; 
    }
}

// ==========================================
// 5. VẼ LÊN MÀN HÌNH (RENDER)
// ==========================================
void PlayingState::render(sf::RenderWindow& window) {
    sf::View currentView = m_camera_.getView();

    // Hiệu ứng rung màn hình khi chết
    if (m_isCameraDeath_ || m_isMonsterDeath_) {
        float offsetX = (std::rand() % 16 - 8) * 1.0f;
        float offsetY = (std::rand() % 16 - 8) * 1.0f;
        currentView.move({offsetX, offsetY});
    }
    
    window.setView(currentView);
    window.clear(sf::Color(40, 40, 40));
    
    // Thứ tự vẽ: Map -> Cửa -> Người
    m_laneManager_.render(window);
    if (m_mode_ == GameMode::CLASSIC && m_finishLineSprite_) {
        window.draw(*m_finishLineSprite_);
    }
    m_player_.render(window);

    if (m_isMonsterDeath_) {
        window.draw(*m_fightEffect_);
    }

    if (m_flashAlpha_ > 0.f) {
        window.draw(m_whiteFlashRect_);
    }

    // Reset View để vẽ HUD không bị dính Camera
    window.setView(window.getDefaultView());
    window.draw(*(m_topOverlaySprite_));

    if (m_hudText_ && !m_introCutscene_.isBusy()) {
        window.draw(*m_hudText_);
        for (int i = 0; i < 4; ++i) {
            window.draw(m_buffIcons_[i]);
            window.draw(m_buffCounts_[i]);
            window.draw(m_buffKeys_[i]);
        }
    }
    m_giantEye.render(window);

    m_introCutscene_.render(window);
}

void PlayingState::initHUD() {
    m_player_.getBuffManager().setCounts(
        m_score_.getMagnetCount(),
        m_score_.getShieldCount(),
        m_score_.getInvisCount(),
        m_score_.getNovaCount()
    );

    const sf::Font& mainFont = ResourceManager::instance().getFont(Config::MAIN_FONT);
    std::string keys[4] = {"1", "Pas", "2", "Spc"};
    std::string texPaths[4] = {
        Config::BUFF_RING_TEXTURE,
        Config::BUFF_SHIELD_TEXTURE,
        Config::BUFF_CLOAK_TEXTURE,
        Config::BUFF_NOVA_TEXTURE
    };

    m_buffIcons_.clear();
    m_buffCounts_.clear();
    m_buffKeys_.clear();

    for (int i = 0; i < 4; ++i) {
        float startX = 20.f + i * 80.f;
        float startY = Config::VIEW_HEIGHT - 60.f; 
        
        m_buffIcons_.emplace_back();
        m_buffIcons_[i].setRadius(25.f);
        
        const sf::Texture& tex = ResourceManager::instance().getTexture(texPaths[i]);
        m_buffIcons_[i].setTexture(&tex, true);
        
        m_buffIcons_[i].setFillColor(sf::Color::White); // Trắng bóc để hiện đúng màu ảnh gốc
        m_buffIcons_[i].setOutlineColor(sf::Color::Black);
        m_buffIcons_[i].setOutlineThickness(2.f);
        m_buffIcons_[i].setPosition({startX, startY}); 
        
        m_buffCounts_.emplace_back(mainFont);
        m_buffCounts_[i].setCharacterSize(18);
        m_buffCounts_[i].setFillColor(sf::Color::White);
        m_buffCounts_[i].setOutlineColor(sf::Color::Black);
        m_buffCounts_[i].setOutlineThickness(2.f);
        m_buffCounts_[i].setPosition({startX + 35.f, startY + 30.f});
        
        m_buffKeys_.emplace_back(mainFont);
        m_buffKeys_[i].setString(keys[i]);
        m_buffKeys_[i].setCharacterSize(16);
        m_buffKeys_[i].setFillColor(sf::Color::White);
        m_buffKeys_[i].setOutlineColor(sf::Color::Black);
        m_buffKeys_[i].setOutlineThickness(2.f);
        m_buffKeys_[i].setPosition({startX + 10.f, startY + 5.f});
    }
}

void PlayingState::syncBuffsToScore() {
    m_score_.setMagnetCount(m_player_.getBuffManager().getMagnetCount());
    m_score_.setShieldCount(m_player_.getBuffManager().getShieldCount());
    m_score_.setInvisCount(m_player_.getBuffManager().getInvisCount());
    m_score_.setNovaCount(m_player_.getBuffManager().getNovaCount());
}