#pragma once

#include "states/state.hpp"
#include "entities/player.hpp"
#include "entities/giant_eye.hpp"
#include "gameplay/lane_manager.hpp"
#include "gameplay/camera_controller.hpp"
#include "gameplay/score_manager.hpp"
#include "gameplay/intro_cutscene.hpp"
#include "data/game_snapshot.hpp"

// ==========================================
// PLAYING STATE
// Gio chi con la mot "orchestrator" - dieu phoi cac manager chuyen trach
// (CameraController, LaneManager, ScoreManager, CollisionSystem) thay vi
// tu lam tat ca moi thu nhu ban goc.
// ==========================================
class PlayingState : public State {
public:
    // New game
    PlayingState(GameMode gameMode, int level = 0);

    // Load game: khoi tao lai tu du lieu da luu
    explicit PlayingState(const GameSnapshot& snapshot);

    void processEvents(Game* game, const std::optional<sf::Event>& event) override;
    void update(Game* game, float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    GameMode m_mode_;
    int m_currentLevel_;
    bool isGameStarted_ = false;

    bool m_isMonsterDeath_ = false;
    std::unique_ptr<sf::Text> m_fightEffect_;

    std::unique_ptr<sf::Sprite> m_finishLineSprite_;
    float m_finishLineY_; // Tọa độ Y của vạch đích
    
    bool m_isVictory_ = false;   // Cờ báo hiệu đã chạm đích
    float m_victoryTimer_ = 0.f; // Thời gian chờ trước khi văng ra màn hình kết thúc

    bool m_isCameraDeath_ = false;   // Cờ đánh dấu đang bị camera nuốt
    float m_shakeTimer_ = 0.5f;      // Thời gian rung màn hình (0.5 giây)

    LevelData m_currentLevelData_; 
    int m_coinsCollectedInRun_ = 0; // Đếm số vàng ăn được trong ván này
    int m_minGridY_ = 9999;

    std::unique_ptr<sf::Text> m_hudText_;

    std::unique_ptr<sf::Sprite> m_topOverlaySprite_;

    IntroCutscene m_introCutscene_;
    CameraController m_camera_;
    LaneManager m_laneManager_;
    Player m_player_;
    ScoreManager m_score_;
    GiantEye m_giantEye;
};
