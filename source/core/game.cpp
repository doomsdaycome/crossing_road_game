#include "core/game.hpp"
#include "states/menu_state.hpp"
#include <iostream> // Thêm thư viện này để log trạng thái

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
            
            // ==========================================
            // MỚI: Bắt sự kiện Window Focus
            // ==========================================
            if (event->is<sf::Event::FocusLost>()) {
                m_isFocused_ = false; 
                std::cout << "[SYSTEM] Game mat tieu diem (Pause)!\n";
            }
            if (event->is<sf::Event::FocusGained>()) {
                m_isFocused_ = true;  
                std::cout << "[SYSTEM] Game co tieu diem (Resume)!\n";
            }

            // ==========================================
            // CHỈ XỬ LÝ SỰ KIỆN KHI GAME ON TOP
            // ==========================================
            if (m_isFocused_) {
                State* m_currentState = this->getCurrentState();
                if (m_currentState) m_currentState->processEvents(this, event);
                
                if (m_currentState != this->getCurrentState()) {
                    break;
                }
            }
        }

        float deltaTime = gameClock.restart().asSeconds();

        // Nhờ đoạn giới hạn này của bạn, khi Focus lại game sẽ không bị vọt deltaTime
        const float MAX_DT = 0.1f;
        if (deltaTime > MAX_DT) deltaTime = MAX_DT;

        // ==========================================
        // CHỈ UPDATE LOGIC KHI GAME ON TOP
        // ==========================================
        if (m_isFocused_) {
            State* m_currentState = this->getCurrentState();
            if (m_currentState) m_currentState->update(this, deltaTime);
        }
        
        // Vòng lặp Render VẪN CHẠY để giữ nguyên hình ảnh màn hình khi bị mất focus
        m_window_.clear(sf::Color::Black);
        for (auto state : m_states_) {
            if (state) state->render(m_window_);
        }
        m_window_.display();
    }
}