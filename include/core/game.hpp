#pragma once
#include <SFML/Graphics.hpp>
#include "states/state.hpp"
#include "core/config.hpp"
#include <time.h>

class Game {
private:
    sf::RenderWindow m_window_;
    bool m_isRunning_;
    std::vector<State*> m_states_;
    sf::Clock gameClock;

    bool m_isFocused_ = true;

public:
    Game();
    ~Game();
    
    void run();
    
    void changeState(State* newState, bool clearAll = false); 
    void pushState(State* newState);
    void popState();

    State* getCurrentState() const {
        return m_states_.empty() ? nullptr : m_states_.back();
    }
    sf::RenderWindow& getWindow() { return m_window_; }
    bool isFocused() const { return m_isFocused_; }
};