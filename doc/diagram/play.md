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

    class CameraController {
        -sf::View view_
        +update(dt, speedMultiplier) void
        +getView() sf::View
    }

    class LaneManager {
        -vector~unique_ptr~Lane~~ lanes_
        +update(dt, isEndless, ...) void
        +render(window) void
        +getLanes() vector~unique_ptr~Lane~~
    }

    class Lane {
        -LaneType type_
        -float yPosition_
        -vector~Monster~ monsters_
        -vector~Carpet~ carpets_
        -vector~ItemData~ items_
        +update(deltaTime, isRedLight) void
        +render(window) void
        +getMonsters() vector~Monster~
    }

    class Monster {
        -sf::Vector2f m_logicalPos_
        -float m_speed_
        -int m_direction_
        +update(deltaTime, isRedLight) void
        +render(window) void
        +getGlobalBounds() sf::FloatRect
    }

    class Player {
        -int m_gridX_
        -int m_gridY_
        +update(dt) void
        +render(window) void
    }

    class GiantEye {
        -State m_state
        +update(dt) void
        +render(window) void
    }

    LaneManager *-- Lane 
    Lane *-- Monster

    PlayingState *-- CameraController
    PlayingState *-- LaneManager
    PlayingState *-- Player
    PlayingState *-- GiantEye

