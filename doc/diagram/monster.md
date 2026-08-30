classDiagram
    class Monster {
        -Vector2f m_logicalPos_
        -float m_speed_
        -int m_direction_
        +Monster(texture, startX, startY, speed, direction, frameCount)
        +update(deltaTime, isRedLight) void
        +render(window) void
        +getPositionX() float
        +isOffScreen() bool
        +getGlobalBounds() FloatRect
    }