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
    inline constexpr int   GRASS_MONSTER_FRAME_COUNT = 1;
    inline constexpr float MONSTER_SPAWN_OFFSET_X = 150.f;
    inline constexpr float MONSTER_SPAWN_START_LEFT  = -100.f;
    inline constexpr float MONSTER_SPAWN_START_RIGHT = 1124.f;
    inline constexpr float MONSTER_OFFSCREEN_LEFT  = -200.f;
    inline constexpr float MONSTER_OFFSCREEN_RIGHT = 1224.f;

    // ---------- ENDLESS MODE ----------
    // FLAPPY-BIRD STYLE: sinh TUNG lane mot (khong con sinh theo batch) moi khi
    // camera con cach lane xa nhat mot khoang ENDLESS_GENERATE_THRESHOLD - giong
    // cach Flappy Bird luon giu 1 khoang cach co dinh phia truoc de sinh ong tiep theo.
    inline constexpr int   ENDLESS_INITIAL_LANE_COUNT = 15;
    inline constexpr float ENDLESS_GENERATE_THRESHOLD  = 300.f;
    inline constexpr float ENDLESS_LANE_CULL_MARGIN    = 200.f;
    // LUU Y: gia tri nay ap dung MOI LAN 1 lane duoc sinh (khong con theo batch 5 lane nhu ban cu).
    // Ban cu: +0.05 moi 5 lane (~+0.01/lane). Gio dat truc tiep +0.01/lane de giu nguyen toc do
    // tang dan nhu cu - neu muon game kho nhanh hon, tang gia tri nay len.
    // TODO (CHANGE PATH/VALUE): tinh chinh do kho theo thoi gian
    inline constexpr float ENDLESS_SPEED_INCREMENT     = 0.01f;
    inline constexpr float CAMERA_BASE_SCROLL_SPEED    = 50.f;

    // TODO (CHANGE PATH/VALUE): so luong lane "nghi chan" (khong quai) ngay truoc mat
    // nguoi choi khi vua vao Endless mode, cho nguoi choi vai giay lam quen truoc khi gap quai.
    inline constexpr int REST_LANE_COUNT_AT_START = 5;

    // ---------- LANE NGHI CHAN XUYEN SUOT GAME (kieu Crossy Road that) ----------
    // Crossy Road that KHONG bao gio de nguoi choi gap qua nhieu lane nguy hiem
    // (co xe/quai) lien tiep ma khong co it nhat 1 lane "an toan" xen giua - dieu nay
    // giu game luon CHOI DUOC (khong bi bit duong bat kha thi) du toc do cuon tang dan.
    //
    // Luat don gian:
    // 1) Random: moi lane co REST_LANE_CHANCE_PERCENT % co hoi la lane nghi chan.
    // 2) Bat buoc: neu da co qua MAX_CONSECUTIVE_HAZARD_LANES lane nguy hiem lien tiep
    //    ma chua "trung" duoc lane nghi chan nao, lane tiep theo se BI EP thanh nghi chan.
    // TODO (CHANGE PATH/VALUE): tang REST_LANE_CHANCE_PERCENT / giam MAX_CONSECUTIVE_HAZARD_LANES
    // de game de tho hon, va nguoc lai de tang do kho.
    inline constexpr int REST_LANE_CHANCE_PERCENT      = 18; // % ngau nhien moi lane la nghi chan
    inline constexpr int MAX_CONSECUTIVE_HAZARD_LANES  = 3;  // toi da 3 lane nguy hiem lien tiep

    // ---------- RAI THOI GIAN NHA QUAI GIUA CAC LANE ----------
    // Neu khong co bo lech nay, cac lane duoc sinh gan nhu cung luc se co spawnTimer_
    // bat dau tu 0 giong het nhau -> quai xuat hien dong loat tren toan man hinh (khong tu nhien).
    // Voi bo lech nay: lane sinh TRUOC se nha quai SOM hon (gan nhu ngay lap tuc),
    // lane sinh SAU se nha quai cham hon theo dung thu tu sinh ra.
    // TODO (CHANGE PATH/VALUE): tang/giam de thay doi do "so le" giua cac lane
    inline constexpr float SPAWN_STAGGER_STEP  = 0.25f;
    // So luong lane trong 1 chu ky rai deu truoc khi lap lai (tranh do tre cong don vo han
    // khi Endless mode chay rat lau va sinh ra hang tram lane).
    inline constexpr int   SPAWN_STAGGER_SLOTS = 6;

    // ---------- ASSET PATHS ----------
    inline const std::string TOP_OVERLAY_TEXTURE    = "asset/images/bg0.png";
    inline const std::string BG_TEXTURE             = "asset/images/bg1.png";
    // TODO (CHANGE PATH/VALUE): doi lai duong dan asset cho dung du an cua ban
    inline const std::string ROAD_BG_TEXTURE        = "asset/images/ln0.png";
    inline const std::string ROAD_MONSTER_TEXTURE   = "asset/images/mt1.png";
    inline const std::string GRASS_BG_TEXTURE       = "asset/images/ln1.png";
    inline const std::string GRASS_MONSTER_TEXTURE  = "asset/images/mt2.png";
    // TODO (CHANGE PATH/VALUE): anh nen cho lane nghi chan (khong quai) - co the tai su dung anh grass
    inline const std::string REST_BG_TEXTURE        = "asset/images/ln2.png";
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
        {1, 1, 2},
        {1, 2, 1, 1},
        {2, 1}
    };
}
