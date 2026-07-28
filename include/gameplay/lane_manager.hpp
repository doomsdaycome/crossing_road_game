#pragma once

#include <vector>
#include <memory>
#include "entities/lane.hpp"
#include "data/level_data.hpp"
#include "core/config.hpp"

// ==========================================
// LANE MANAGER
// So huu toan bo Lane, xu ly sinh lane (Endless mode) va don rac (culling).
// Tach ra khoi PlayingState de tuan thu SRP - PlayingState khong con phai
// biet chi tiet cach lane duoc sinh/xoa nua.
// ==========================================
class LaneManager {
public:
    // Khoi tao cho Classic mode: dung lane co dinh tu du lieu level
    void buildFromLevel(const LevelData& levelData);

    // Khoi tao cho Endless mode: sinh san 1 luong lane ban dau
    void initEndless();

    // Khoi tao tu du lieu save game (Load Game)
    void buildFromSaveData(const std::vector<LaneSaveData>& savedLanes);

    void update(float dt, bool isEndlessMode, float viewTopEdge, float viewBottomEdge);
    void render(sf::RenderWindow& window);

    std::vector<LaneSaveData> exportSaveData() const;

    const std::vector<std::unique_ptr<Lane>>& getLanes() const;

    float getGlobalSpeedMultiplier() const;

private:
    void generateEndlessLanes(int count);

    std::vector<std::unique_ptr<Lane>> lanes_;
    float highestLaneY_ = Config::VIEW_HEIGHT;
    float globalSpeedMultiplier_ = 1.f;
};
