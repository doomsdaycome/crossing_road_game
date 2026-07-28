#pragma once

#include <vector>
#include <string>

// ==========================================
// CONFIG: Toan bo hang so va gia tri can bang game (balancing)
// Gom ve mot cho de de chinh sua, tranh "magic numbers" rai rac khap code.
// ==========================================
namespace Config {

    // ---------- WINDOW INFO ------------
    inline const std::string WINDOW_TITLE = "Crossing Road Game";
    inline constexpr int WINDOW_WIDTH = 1024;
    inline constexpr int WINDOW_HEIGHT = 768;

    // ---------- CAMERA / VIEW ----------
    inline constexpr float VIEW_WIDTH   = 1024.f;
    inline constexpr float VIEW_HEIGHT  = 768.f;

    // ---------- GRID / TILE ----------
    inline constexpr float TILE_SIZE = 64.f;

    // ---------- PLAYER ----------
    inline constexpr float PLAYER_SPEED = 200.f;
    // TODO (CHANGE PATH/VALUE): toa do luoi xuat phat cua nhan vat
    inline constexpr int PLAYER_START_GRID_X = 5;
    inline constexpr int PLAYER_START_GRID_Y = 10;
    // Gioi han di chuyen ngang cua nguoi choi (so o luoi) - fix bug ban goc: di ra ngoai vo han
    inline constexpr int PLAYER_MIN_GRID_X = 0;
    inline constexpr int PLAYER_MAX_GRID_X = 15;

    // ---------- LANE ----------
    inline constexpr float LANE_HEIGHT = 64.f;
    inline constexpr float ROAD_BASE_SPEED = 150.f;
    inline constexpr float GRASS_BASE_SPEED = 200.f;
    inline constexpr float ROAD_SPAWN_INTERVAL = 3.5f;
    inline constexpr float GRASS_SPAWN_INTERVAL = 2.0f;
    inline constexpr int   ROAD_MONSTER_FRAME_COUNT = 1;
    inline constexpr int   GRASS_MONSTER_FRAME_COUNT = 4;
    inline constexpr float MONSTER_SPAWN_OFFSET_X = 150.f;
    inline constexpr float MONSTER_SPAWN_START_LEFT  = -100.f;
    inline constexpr float MONSTER_SPAWN_START_RIGHT = 1124.f;
    inline constexpr float MONSTER_OFFSCREEN_LEFT  = -200.f;
    inline constexpr float MONSTER_OFFSCREEN_RIGHT = 1224.f;

    // ---------- ENDLESS MODE ----------
    inline constexpr int   ENDLESS_INITIAL_LANE_COUNT = 15;
    inline constexpr int   ENDLESS_GENERATE_BATCH      = 5;
    inline constexpr float ENDLESS_GENERATE_THRESHOLD  = 300.f;
    inline constexpr float ENDLESS_LANE_CULL_MARGIN    = 200.f;
    inline constexpr float ENDLESS_SPEED_INCREMENT     = 0.05f;
    inline constexpr float CAMERA_BASE_SCROLL_SPEED    = 50.f;

    // ---------- ASSET PATHS ----------
    // TODO (CHANGE PATH/VALUE): doi lai duong dan asset cho dung du an cua ban
    inline const std::string ROAD_BG_TEXTURE        = "asset/images/ln0.png";
    inline const std::string ROAD_MONSTER_TEXTURE   = "asset/images/mt1.png";
    inline const std::string GRASS_BG_TEXTURE       = "asset/images/ln1.png";
    inline const std::string GRASS_MONSTER_TEXTURE  = "asset/images/mt2.png";
    inline const std::string PLAYER_TEXTURE         = "asset/images/pl0.png";
    // TODO (CHANGE PATH/VALUE): font chu dung cho man hinh Game Over / UI
    inline const std::string MAIN_FONT              = "asset/fonts/font.otf";

    // TODO (CHANGE PATH/VALUE): duong dan thu muc chua level JSON
    inline const std::string LEVEL_PATH_PREFIX = "asset/levels/level_";
    inline const std::string LEVEL_PATH_SUFFIX  = ".json";

    // TODO (CHANGE PATH/VALUE): duong dan file save mac dinh
    inline const std::string DEFAULT_SAVE_PATH = "save/save_game.json";

    // ---------- SPAWN PATTERNS ----------
    // Moi pattern la so luong quai xuat hien moi lan "nha" quai, lap vong.
    // TODO (CHANGE PATH/VALUE): tinh chinh do kho bang cach doi cac pattern nay
    inline const std::vector<std::vector<int>> SPAWN_PATTERNS = {
        {1},
        {1, 1, 2},
        {2, 1, 1},
        {1, 2, 1, 1},
    };
}
