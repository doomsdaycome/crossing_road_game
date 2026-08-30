classDiagram
    direction TD

    class PlayingState {
        -CameraController m_camera_
        -LaneManager m_laneManager_
        -Player m_player_
        -ScoreManager m_score_
        -GiantEye m_giantEye
        +processEvents(Game* game, event) void
        +update(Game* game, float dt) void
        +render(window) void
    }

    class ScoreManager {
        -int score_
        -int bestScore_
        -int totalCoins_
        +addPoints(points) void
        +loadHighScore() void
        +saveHighScore() void
    }

    class SaveGameRepository {
        <<static>>
        +save(filepath, snapshot) bool
        +load(filepath) optional~GameSnapshot~
    }

    class LevelRepository {
        <<static>>
        +loadLevel(filepath) LevelData
        +saveLevel(filepath, data) bool
    }

    class CollisionSystem {
        <<static>>
        +checkPlayerVsMonsters(player, laneManager) bool
        +checkAndCollectCoins(player, laneManager) int
    }

    PlayingState *-- ScoreManager
    PlayingState ..> SaveGameRepository
    PlayingState ..> LevelRepository
    PlayingState ..> CollisionSystem