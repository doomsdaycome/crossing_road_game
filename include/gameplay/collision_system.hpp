#pragma once

class Player;
class LaneManager;

// ==========================================
// COLLISION SYSTEM
// He thong thuan logic (stateless): chi DOC du lieu tu Player/LaneManager
// va tra loi cau hoi va cham. Khong so huu, khong sua doi entity nao.
// Day la phan logic BI THIEU HOAN TOAN o ban goc.
// ==========================================
namespace CollisionSystem {
    // Tra ve true neu Player va cham voi bat ky Monster nao dang hoat dong
    bool checkPlayerVsMonsters(const Player& player, const LaneManager& laneManager);
}
