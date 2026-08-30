classDiagram
    direction TD

    class State {
        <<abstract>>
        +processEvents(Game* game, event) void*
        +update(Game* game, float dt) void*
        +render(sf::RenderWindow& window) void*
    }

    class MenuState {
        +processEvents(Game* game, event) void
        +update(Game* game, float dt) void
        +render(window) void
    }

    class PlayingState {
        -GameMode m_mode_
        -int m_currentLevel_
        -Player m_player_
        +processEvents(Game* game, event) void
        +update(Game* game, float dt) void
        +render(window) void
    }

    class ModeState {
        +processEvents(Game* game, event) void
        +update(Game* game, float dt) void
        +render(window) void
    }

    class LevelState {
        -int m_currentPage_
        +processEvents(Game* game, event) void
        +update(Game* game, float dt) void
        +render(window) void
    }

    State <|-- MenuState
    State <|-- PlayingState
    State <|-- ModeState
    State <|-- LevelState

classDiagram
    direction TD

    class State {
        <<abstract>>
        +processEvents(Game* game, event) void*
        +update(Game* game, float dt) void*
        +render(sf::RenderWindow& window) void*
    }

    class SettingState {
        -bool m_isFromGameplay_
        +processEvents(Game* game, event) void
        +update(Game* game, float dt) void
        +render(window) void
    }

    class ShopState {
        -ShopPage m_currentPage_
        +processEvents(Game* game, event) void
        +update(Game* game, float dt) void
        +render(window) void
    }

    class TutorialState {
        +processEvents(Game* game, event) void
        +update(Game* game, float dt) void
        +render(window) void
    }

    class ScoreboardState {
        +processEvents(Game* game, event) void
        +update(Game* game, float dt) void
        +render(window) void
    }

    State <|-- SettingState
    State <|-- ShopState
    State <|-- TutorialState
    State <|-- ScoreboardState

classDiagram
    direction TD

    class State {
        <<abstract>>
        +processEvents(Game* game, event) void*
        +update(Game* game, float dt) void*
        +render(sf::RenderWindow& window) void*
    }

    class GameOverState {
        -bool m_isVictory_
        -int m_finalScore_
        +processEvents(Game* game, event) void
        +update(Game* game, float dt) void
        +render(window) void
    }

    class SaveState {
        +processEvents(Game* game, event) void
        +update(Game* game, float dt) void
        +render(window) void
    }

    class LoadState {
        +processEvents(Game* game, event) void
        +update(Game* game, float dt) void
        +render(window) void
    }

    State <|-- GameOverState
    State <|-- SaveState
    State <|-- LoadState

classDiagram
    direction TD

    class Game {
        -sf::RenderWindow m_window_
        -bool m_isRunning_
        -vector~State*~ m_states_
        -sf::Clock gameClock
        -bool m_isFocused_
        +run() void
        +changeState(newState, clearAll) void
        +pushState(newState) void
        +popState() void
        +getCurrentState() State*
        +getWindow() sf::RenderWindow&
        +isFocused() bool
    }

    class State {
        <<interface>>
        +~State()*
        +processEvents(game, event)* void
        +update(game, dt)* void
        +render(window)* void
    }


    Game "1" o-- "*" State : manages stack (m_states_)
    State ..> Game : receives pointer via updates