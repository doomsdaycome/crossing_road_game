#pragma once

#include "states/state.hpp"
#include "entities/player.hpp"
#include "gameplay/lane_manager.hpp"
#include "gameplay/camera_controller.hpp"
#include "gameplay/score_manager.hpp"
#include "data/game_snapshot.hpp"

enum class GameMode {
    CLASSIC,
    ENDLESS
};

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

    CameraController m_camera_;
    LaneManager m_laneManager_;
    Player m_player_;
    ScoreManager m_score_;
};
