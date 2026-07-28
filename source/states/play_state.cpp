#include "states/play_state.hpp"
#include "states/setting_state.hpp"
#include "states/game_over_state.hpp"
#include "core/game.hpp"
#include "data/level_data.hpp"
#include "core/config.hpp"
#include "services/level_repository.hpp"
#include "services/save_game_repository.hpp"
#include "gameplay/collision_system.hpp"
#include <iostream>

// ==========================================
// CONSTRUCTOR: NEW GAME
// ==========================================
PlayingState::PlayingState(GameMode gameMode, int level) {
    m_mode_ = gameMode;
    m_currentLevel_ = level;

    if (m_mode_ == GameMode::CLASSIC) {
        // TODO (CHANGE PATH/VALUE): duong dan thu muc chua file level JSON (xem Config::LEVEL_PATH_PREFIX)
        std::string filepath = Config::LEVEL_PATH_PREFIX + std::to_string(m_currentLevel_) + Config::LEVEL_PATH_SUFFIX;
        LevelData levelData = LevelRepository::loadLevel(filepath);
        m_laneManager_.buildFromLevel(levelData);
    }
    else if (m_mode_ == GameMode::ENDLESS) {
        m_laneManager_.initEndless();
    }
}

// ==========================================
// CONSTRUCTOR: LOAD GAME (tu GameSnapshot da doc boi SaveGameRepository)
// ==========================================
PlayingState::PlayingState(const GameSnapshot& snapshot) {
    // TODO (CHANGE PATH/VALUE): neu muon ho tro Load Game cho ca Classic mode,
    // hay luu them GameMode vao GameSnapshot. Hien tai mac dinh ve ENDLESS
    // vi cac Lane duoc phuc hoi nguyen trang tu du lieu da luu.
    m_mode_ = GameMode::ENDLESS;
    m_currentLevel_ = 0;

    m_laneManager_.buildFromSaveData(snapshot.lanes);
    m_player_.loadState(snapshot.playerGridX, snapshot.playerGridY);
    m_score_.loadScore(snapshot.score);
}

void PlayingState::processEvents(Game* game, const std::optional<sf::Event>& event) {
    if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPress->code == sf::Keyboard::Key::Escape) {
            game->pushState(new SettingState());
            return;
        }

        // MOI: F5 = Save, F9 = Load - minh hoa SaveGameRepository hoat dong that su
        if (keyPress->code == sf::Keyboard::Key::F5) {
            GameSnapshot snapshot;
            snapshot.score = m_score_.getScore();
            snapshot.playerGridX = m_player_.getGridX();
            snapshot.playerGridY = m_player_.getGridY();
            snapshot.lanes = m_laneManager_.exportSaveData();

            // TODO (CHANGE PATH/VALUE): duong dan file save (xem Config::DEFAULT_SAVE_PATH)
            if (SaveGameRepository::save(Config::DEFAULT_SAVE_PATH, snapshot)) {
                std::cout << "Da luu game thanh cong!\n";
            }
            return;
        }

        if (keyPress->code == sf::Keyboard::Key::F9) {
            auto loaded = SaveGameRepository::load(Config::DEFAULT_SAVE_PATH);
            if (loaded.has_value()) {
                game->changeState(new PlayingState(loaded.value()));
            }
            return;
        }
    }
    m_player_.processEvents(event);
}

void PlayingState::update(Game* game, float dt) {
    // 1. Cap nhat nhan vat
    m_player_.update(dt);

    // 2. Cap nhat camera
    m_camera_.update(dt, m_laneManager_.getGlobalSpeedMultiplier());

    // 3. Cap nhat lane (sinh/xoa lane, di chuyen quai)
    bool isEndless = (m_mode_ == GameMode::ENDLESS);
    m_laneManager_.update(dt, isEndless, m_camera_.getTopEdge(), m_camera_.getBottomEdge());

    // 4. MOI: kiem tra va cham Player vs Monster - hoan toan thieu o ban goc
    if (CollisionSystem::checkPlayerVsMonsters(m_player_, m_laneManager_)) {
        m_player_.die();
        game->changeState(new GameOverState(m_score_.getScore()));
        return;
    }
}

void PlayingState::render(sf::RenderWindow& window) {
    window.setView(m_camera_.getView());
    window.clear(sf::Color(40, 40, 40));

    m_laneManager_.render(window);
    m_player_.render(window);

    window.setView(window.getDefaultView());
}
