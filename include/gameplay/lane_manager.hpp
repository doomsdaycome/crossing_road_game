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

    void update(float dt, bool isEndlessMode, float viewTopEdge, float viewBottomEdge, bool isRedLight);
    void render(sf::RenderWindow& window);

    std::vector<LaneSaveData> exportSaveData() const;

    const std::vector<std::unique_ptr<Lane>>& getLanes() const;

    float getGlobalSpeedMultiplier() const;

    void initBackground(float startY);

private:
    // MOI: sinh TUNG lane mot (kieu Flappy Bird sinh tung ong theo khoang cach co dinh)
    // thay vi sinh 1 batch nhieu lane cung luc nhu ban truoc.
    // forceRest = true -> ep lane nay la LaneType::REST (dung cho vai lane dau tien).
    void spawnOneEndlessLane(bool forceRest);

    // MOI: quyet dinh loai lane tiep theo (ROAD/GRASS/REST) - ket hop random co trong so
    // va gioi han so lane nguy hiem lien tiep, giong cach Crossy Road dam bao man choi
    // luon "choi duoc" (khong bao gio bi bit duong bat kha thi).
    LaneType pickNextLaneType();

    std::vector<std::unique_ptr<Lane>> lanes_;
    float highestLaneY_ = Config::VIEW_HEIGHT;
    float globalSpeedMultiplier_ = 1.f;

    // Bo dem thu tu sinh lane - dung de RAI THOI GIAN nha quai giua cac lane (xem Lane::Lane)
    int spawnOrderCounter_ = 0;

    // MOI: dem so lane nguy hiem (ROAD/GRASS) da sinh LIEN TIEP ke tu lane nghi chan gan nhat
    int consecutiveHazardCount_ = 0;

    std::unique_ptr<sf::Sprite> bgSprite1_;
    std::unique_ptr<sf::Sprite> bgSprite2_;
    float bgHeight_ = 768.f;
};
