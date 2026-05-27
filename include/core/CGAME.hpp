#pragma once
#include <SFML/Graphics.hpp>
#include "states/State.hpp"
#include "system/config.hpp"

class CGAME {
private:
    sf::RenderWindow m_window;
    bool m_isRunning;
    
    // Con trỏ Đa hình lưu giữ màn hình hiện tại
    State* m_currentState; 

public:
    CGAME();
    ~CGAME();
    
    void run();
    
    // Hàm API để các màn hình con gọi khi muốn đổi cảnh
    void changeState(State* newState); 
};