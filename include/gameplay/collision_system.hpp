#pragma once
#include "data/level_data.hpp"

class Player;
class LaneManager;

// ==========================================
// COLLISION SYSTEM
// He thong thuan logic (stateless): chi DOC du lieu tu Player/LaneManager
// va tra loi cau hoi va cham. Khong so huu, khong sua doi entity nao.
// Day la phan logic BI THIEU HOAN TOAN o ban goc.
// ==========================================
class CollisionSystem {
public:
    static bool checkPlayerVsMonsters(Player& player, const LaneManager& laneManager);
    static int checkAndCollectCoins(const Player& player, LaneManager& laneManager);
private:
    CollisionSystem() = delete;
};
