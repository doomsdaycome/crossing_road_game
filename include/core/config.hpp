#pragma once

#include <vector>
#include <string>

// ==========================================
// THÔNG SỐ CẤU HÌNH TRÒ CHƠI (GAME CONFIGURATION)
// Chứa toàn bộ các hằng số, thông số cân bằng game và đường dẫn tài nguyên.
// Việc gom nhóm giúp dễ dàng tinh chỉnh độ khó và tái sử dụng mã nguồn.
// ==========================================
namespace Config {

    // ==========================================
    // 1. HỆ THỐNG HIỂN THỊ & CAMERA
    // ==========================================
    inline const std::string WINDOW_TITLE = "Crossing Castle"; // Tên hiển thị trên thanh tiêu đề cửa sổ
    inline constexpr int WINDOW_WIDTH     = 1024;              // Chiều rộng cửa sổ thực tế (pixel)
    inline constexpr int WINDOW_HEIGHT    = 768;               // Chiều cao cửa sổ thực tế (pixel)
    
    inline constexpr float VIEW_WIDTH     = 1024.f;            // Kích thước chiều ngang của Camera quét trong game
    inline constexpr float VIEW_HEIGHT    = 768.f;             // Kích thước chiều dọc của Camera quét trong game

    // ==========================================
    // 2. KHÔNG GIAN LƯỚI (GRID SYSTEM)
    // ==========================================
    inline constexpr float TILE_SIZE = 64.f; // Kích thước của 1 ô vuông trên bản đồ (64x64 pixel). Mọi di chuyển đều dựa trên ô này

    // ==========================================
    // 3. THÔNG SỐ NHÂN VẬT (PLAYER)
    // ==========================================
    inline constexpr float PLAYER_SPEED        = 600.f; // Tốc độ lướt của nhân vật (pixel/giây). Càng cao, nhân vật nhảy càng dứt khoát
    inline constexpr int   PLAYER_START_GRID_X = 5;     // Ô xuất phát theo trục X (Cột số 5)
    inline constexpr int   PLAYER_START_GRID_Y = 10;    // Ô xuất phát theo trục Y (Hàng số 10)
    
    // Giới hạn không gian di chuyển (Chống văng ra khỏi màn hình)
    inline constexpr int PLAYER_MIN_GRID_X = 0;         // Cột tận cùng bên trái
    inline constexpr int PLAYER_MAX_GRID_X = 15;        // Cột tận cùng bên phải

    // ==========================================
    // 4. QUÁI VẬT & LÀN ĐƯỜNG (OBSTACLES & LANES)
    // Thay vì Road (Đường) và Grass (Cỏ), ta dùng hệ thống Zombie (Đi bộ) và Bat (Dơi bay)
    // ==========================================
    inline constexpr float LANE_HEIGHT = 64.f; // Chiều cao của 1 làn đường, bằng đúng 1 TILE_SIZE
    
    // Tốc độ di chuyển cơ bản (pixel/giây)
    inline constexpr float ZOMBIE_BASE_SPEED = 100.f; // Tốc độ gốc của Zombie đi bộ trên nền đá
    inline constexpr float BAT_BASE_SPEED    = 120.f; // Tốc độ gốc của Dơi bay (nhanh hơn Zombie)
    
    // Số khung hình (Frame) để chạy Animation cho từng loại quái
    inline constexpr int ZOMBIE_FRAME_COUNT = 1; // Số frame ảnh của Zombie
    inline constexpr int BAT_FRAME_COUNT    = 1; // Số frame ảnh của Dơi

    // Tọa độ sinh/hủy quái vật
    inline constexpr float MONSTER_SPAWN_START_LEFT  = -100.f; // Điểm bắt đầu đi ra từ bên trái màn hình
    inline constexpr float MONSTER_SPAWN_START_RIGHT = 1124.f; // Điểm bắt đầu đi ra từ bên phải màn hình
    inline constexpr float MONSTER_OFFSCREEN_LEFT    = -200.f; // Quá vạch này bên trái -> Xóa quái khỏi RAM
    inline constexpr float MONSTER_OFFSCREEN_RIGHT   = 1224.f; // Quá vạch này bên phải -> Xóa quái khỏi RAM

    // ==========================================
    // 5. CƠ CHẾ CUỘN VÔ TẬN (ENDLESS MODE)
    // ==========================================
    inline constexpr float CAMERA_BASE_SCROLL_SPEED   = 30.f;   // Tốc độ Camera tự động trượt lên trên (Ép người chơi phải đi tiếp)
    inline constexpr int   ENDLESS_INITIAL_LANE_COUNT = 15;     // Số lượng làn đường được tạo sẵn ngay khi vừa vào game
    
    // Thuật toán cuốn chiếu (Procedural Generation)
    inline constexpr float ENDLESS_GENERATE_THRESHOLD = 1000.f; // Khoảng cách nhìn trước. Nếu Camera cách mép trên ngần này pixel, tạo đường mới
    inline constexpr float ENDLESS_LANE_CULL_MARGIN   = 200.f;  // Khoảng cách dọn dẹp. Làn đường nào tụt lại sau Camera 200px sẽ bị xóa sổ
    
    // Hệ thống tăng độ khó (Difficulty Curve)
    inline constexpr float ENDLESS_SPEED_INCREMENT = 0.01f;     // Cứ mỗi 1 làn đường mới sinh ra, vận tốc toàn bộ game tăng thêm 1%

    // ==========================================
    // 6. CƠ CHẾ BẢO HIỂM SINH MẠNH (REST LANES)
    // Game luôn chèn các làn an toàn để người chơi có chỗ nghỉ chân
    // ==========================================
    inline constexpr int REST_LANE_COUNT_AT_START     = 5;  // Cho người chơi 5 làn an toàn ở vạch xuất phát để khởi động
    inline constexpr int REST_LANE_CHANCE_PERCENT     = 18; // Mỗi làn sinh ra có 18% tỷ lệ ngẫu nhiên là làn nghỉ chân (Không quái)
    inline constexpr int MAX_CONSECUTIVE_HAZARD_LANES = 3;  // Nếu xui xẻo ra 3 làn nguy hiểm liên tiếp, làn thứ 4 bắt buộc phải là làn an toàn

    // ==========================================
    // 7. THUẬT TOÁN ĐIỀU HƯỚNG NHỊP ĐỘ (SPAWN PATTERNS)
    // ==========================================
    // Tránh tình trạng quái ra đồng loạt cùng lúc trên các làn khác nhau
    inline constexpr int   SPAWN_STAGGER_SLOTS = 6;     // Chia thành 6 chu kỳ lệch nhịp
    inline constexpr float SPAWN_STAGGER_STEP  = 0.25f; // Mỗi chu kỳ xuất phát cách nhau 0.25 giây

    // Các cụm thời gian giãn cách (Gap-based Spawning) - Đơn vị: Giây
    inline const std::vector<std::vector<float>> SPAWN_PATTERNS = {
        {0.5f, 4.0f},               // Nhịp 1: 2 con sát nhau, hở 4 giây
        {0.8f, 0.8f, 3.5f},         // Nhịp 2: 3 con nối đuôi, hở 3.5 giây
        {1.5f, 1.5f, 5.0f},         // Nhịp 3: 3 con tà tà cách đều, hở 5 giây
        {0.2f, 0.2f, 0.2f, 4.5f}    // Nhịp 4: 4 con bám sát đít nhau, hở 4.5 giây
    };

    // ==========================================
    // 8. ĐƯỜNG DẪN TÀI NGUYÊN (ASSET PATHS)
    // ==========================================
    // --- Nền và Phông chữ ---
    inline const std::string MAIN_FONT           = "asset/fonts/font.ttf";       // Phông chữ toàn hệ thống
    inline const std::string TOP_OVERLAY_TEXTURE = "asset/images/world/bg0.png"; // Khung viền hang động trên cùng
    inline const std::string BG_TEXTURE          = "asset/images/world/bg1.png"; // Lớp nền gạch cơ sở
    
    // --- Giao diện (UI) ---
    inline const std::string MENU_BG_TEXTURE     = "asset/images/ui/bg.png";
    inline const std::string OPTION_BG_TEXTURE   = "asset/images/ui/bg_option.png";           
    inline const std::string BTN_BLANK_TEXTURE   = "asset/images/ui/btn_blank.png";
    inline const std::string BTN_BLANK_SQUARE_TEXTURE = "asset/images/ui/btn_square.png";    
    inline const std::string BTN_START_TEXTURE   = "asset/images/ui/btn_start.png";
    inline const std::string DOOR_LEFT_TEXTURE  = "asset/images/ui/door_left.png";
    inline const std::string DOOR_RIGHT_TEXTURE = "asset/images/ui/door_right.png";

    // --- Thực thể & Môi trường ---
    inline const std::string PLAYER_TEXTURE      = "asset/images/entities/pl0.png"; // Hiệp sĩ
    
    inline const std::string ZOMBIE_LANE_TEXTURE = "asset/images/world/ln0.png";    // Nền đá (Cho Zombie)
    inline const std::string ZOMBIE_TEXTURE      = "asset/images/entities/mt1.png"; // Quái Zombie
    
    inline const std::string BAT_LANE_TEXTURE    = "asset/images/world/ln1.png";    // Nền vực/hang (Cho Dơi)
    inline const std::string BAT_TEXTURE         = "asset/images/entities/mt2.png"; // Quái Dơi
    
    inline const std::string REST_LANE_TEXTURE   = "asset/images/world/ln2.png";    // Nền an toàn

    inline const std::string FINISH_LINE_TEXTURE = "asset/images/world/bg2.png";

    inline const std::string COIN_1_TEXTURE      = "asset/images/ui/item_coin_1.png";
    inline const std::string COIN_3_TEXTURE      = "asset/images/ui/item_coin_3.png";
    inline const std::string TREASURE_TEXTURE    = "asset/images/ui/item_treasure.png";

    // --- Nhạc & SFX ---
    inline const std::string BG_MUSIC            = "asset/audio/bg_music.ogg";
    inline const std::string BGM_PLAYING         = "asset/audio/pl_music.ogg";

    inline const std::string EYE_FRAME_0 = "asset/images/entities/Sharinga/eye_frame_0.png";
    inline const std::string EYE_FRAME_1 = "asset/images/entities/Sharinga/eye_frame_1.png";
    inline const std::string EYE_FRAME_2 = "asset/images/entities/Sharinga/eye_frame_2.png";
    inline const std::string EYE_FRAME_3 = "asset/images/entities/Sharinga/eye_frame_3.png";
    inline const std::string EYE_FRAME_4 = "asset/images/entities/Sharinga/eye_frame_4.png";
    inline const std::string EYE_FRAME_5 = "asset/images/entities/Sharinga/eye_frame_5.png";

    // ==========================================
    // 9. DỮ LIỆU I/O (TỆP TIN LƯU TRỮ)
    // ==========================================
    inline const std::string LEVEL_PATH_PREFIX = "asset/levels/level_"; // Tiền tố load bản đồ Classic
    inline const std::string LEVEL_PATH_SUFFIX = ".json";               // Định dạng file bản đồ
    inline const std::string DEFAULT_SAVE_PATH = "data/save.json"; // File lưu game
}