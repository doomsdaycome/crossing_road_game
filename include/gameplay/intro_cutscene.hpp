#pragma once
#include <SFML/Graphics.hpp>

class IntroCutscene {
public:
    // Định nghĩa các trạng thái của cutscene
    enum class Status { LOADING, OPENING, IDLE, CLOSING, CLOSED };

private:
    Status m_status_;
    float m_loadingProgress_;
    float m_doorProgress_; // 1.0 là Đóng kín, 0.0 là Mở toang

public:
    // startIdle = true: Dùng cho Menu (Cửa mở sẵn, nằm im)
    // startIdle = false: Dùng cho PlayingState (Vào game là đóng kín và chạy Loading)
    IntroCutscene(bool startIdle = false);
    
    void update(float dt);
    void render(sf::RenderWindow& window);
    
    void startClosing(); // Kích hoạt đóng cửa
    
    bool isFinishedOpening() const;
    bool isFinishedClosing() const;
    bool isBusy() const; // Kiểm tra xem cửa có đang chuyển động không
};