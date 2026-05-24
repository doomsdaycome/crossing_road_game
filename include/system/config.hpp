#pragma once
#include <string>

namespace Config {
    // 1. Cấu hình Cửa sổ Game (Window Settings)
    inline constexpr int WINDOW_WIDTH = 1024;
    inline constexpr int WINDOW_HEIGHT = 768;
    inline const std::string GAME_TITLE = "Crossing Game - Nhom 5";
    inline constexpr int FRAME_RATE_LIMIT = 60; // Giới hạn FPS giúp game chạy mượt, đều tốc độ

    // 2. Cấu hình Logic Trò chơi (Gameplay Settings)
    inline constexpr int MAX_LEVEL = 5;
    inline constexpr float PLAYER_SPEED = 5.0f;
    inline constexpr float INITIAL_NPC_SPEED = 2.0f;
    inline constexpr float SPEED_MULTIPLIER_PER_LEVEL = 1.5f; // Tăng độ khó khi lên cấp

    // 3. Kích thước Nhân vật / Vật thể (Bounding Box kích thước để tính va chạm)
    // Giúp Người 3 và Người 2 đồng bộ kích thước pixel của Sprite ảnh
    inline constexpr float PLAYER_WIDTH = 40.0f;
    inline constexpr float PLAYER_HEIGHT = 40.0f;
    inline constexpr float CAR_WIDTH = 80.0f;
    inline constexpr float CAR_HEIGHT = 40.0f;
    inline constexpr float TRUCK_WIDTH = 120.0f;
    inline constexpr float TRUCK_HEIGHT = 50.0f;

    // 4. Cấu hình Hệ thống Đèn Giao thông (Yêu cầu 4.4)
    inline constexpr float RED_LIGHT_DURATION = 3.0f;  // Đèn đỏ dừng 3 giây
    inline constexpr float GREEN_LIGHT_DURATION = 5.0f; // Đèn xanh chạy 5 giây
}