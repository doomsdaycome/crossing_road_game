#include "core/CGAME.hpp"
#include "states/MenuState.hpp"

CGAME::CGAME() : m_isRunning(true), m_currentState(nullptr) {
    m_window.create(sf::VideoMode({Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}), Config::GAME_TITLE);
    
    // Khởi tạo trạng thái đầu tiên là Menu
    changeState(new MenuState()); 
}

CGAME::~CGAME() {
    if (m_currentState) {
        delete m_currentState;
    }
}

void CGAME::changeState(State* newState) {
    if (m_currentState) {
        delete m_currentState; // Xóa màn hình cũ khỏi RAM
    }
    m_currentState = newState; // Gắn màn hình mới vào
}

void CGAME::run() {
    while (m_window.isOpen() && m_isRunning) {
        // CÚ PHÁP SFML 3
        while (const std::optional<sf::Event> event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) m_window.close();
            
            // Ủy quyền việc bắt phím cho màn hình hiện tại xử lý
            if (m_currentState) m_currentState->processEvents(this, event);
        }

        // Ủy quyền việc cập nhật và vẽ cho màn hình hiện tại
        if (m_currentState) m_currentState->update(this);
        
        m_window.clear(sf::Color::Black);
        if (m_currentState) m_currentState->render(m_window);
        m_window.display();
    }
}