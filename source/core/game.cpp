#include "core/game.hpp"
#include "states/menu_state.hpp"

Game::Game() {
    m_isRunning_ = true;
    m_window_.create(sf::VideoMode({Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}), Config::WINDOW_TITLE);
    
    this->pushState(new MenuState());
}

Game::~Game() {
    while (!m_states_.empty()) {
        delete m_states_.back();
        m_states_.pop_back();
    }
}

void Game::changeState(State* newState, bool clearAll) {
    if (clearAll) {
        while (!m_states_.empty()) {
            delete m_states_.back();
            m_states_.pop_back();
        }
    } else {
        if (!m_states_.empty()) {
            delete m_states_.back();
            m_states_.pop_back();
        }
    }

    m_states_.push_back(newState);
}

void Game::pushState(State* newState) {
    if (!m_states_.empty()) {
        // m_states_.back()->pause(); Dừng logic của state cũ
    }

    m_states_.push_back(newState);
}

void Game::popState() {
    if (!m_states_.empty()) {
        delete m_states_.back();
        m_states_.pop_back();
    }

    if (!m_states_.empty()) {
        // m_states.back()->resume(); Chạy logic của state cũ
    }
}

void Game::run() {
    while (m_window_.isOpen() && m_isRunning_) {

        while (const std::optional<sf::Event> event = m_window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) m_window_.close();
            
            State* m_currentState = this->getCurrentState();
            if (m_currentState) m_currentState->processEvents(this, event);
            if (m_currentState != this->getCurrentState()) {
                break;
            }
        }

        float deltaTime = gameClock.restart().asSeconds();

        const float MAX_DT = 0.1f;
        if (deltaTime > MAX_DT) deltaTime = MAX_DT;

        State* m_currentState = this->getCurrentState();
        if (m_currentState) m_currentState->update(this, deltaTime);
        
        m_window_.clear(sf::Color::Black);
        for (auto state : m_states_) {
            if (state) state->render(m_window_);
        }
        m_window_.display();
    }
}