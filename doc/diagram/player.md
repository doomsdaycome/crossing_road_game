classDiagram
    class Player {
        -int m_gridX_
        -int m_gridY_
        -float m_speed_
        -bool m_isAlive_
        -bool m_isMoving_
        -bool m_isRiding_
        -bool m_isJumping_

        +processEvents(event, checkChasmFunc) void
        +update(dt) void
        +render(window) void
        +getGridX() int
        +getGridY() int
        +getBuffManager() BuffManager
        +getGlobalBounds() FloatRect
        +loadState(gridX, gridY) void
        +isAlive() bool
        +die() void
        +setOnDeath(callback) void
        +setRiding(isRiding, speed) void
        +setSkin(skinId) void

    }