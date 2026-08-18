#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "entities/monster.hpp"
#include "entities/carpet.hpp"

// Loai lane. enum class de de mo rong (them WATER, RAIL...) sau nay
// ma khong phai sua if/else rai rac khap noi (fix OCP violation trong audit).
enum class LaneType {
    ROAD = 0,
    GRASS = 1,
    REST = 2, // MOI: lane "nghi chan" - KHONG nha quai, dung o dau Endless mode
    CHASM = 3 // Lane Vực Thẳm, thả thảm bay
};

enum class ItemType { COIN_1 = 1, COIN_3 = 2, TREASURE = 3 };

struct ItemData {
    ItemType type;
    float x; // Tọa độ X cụ thể trên Lane
};

// DTO dung de luu/khoi phuc trang thai 1 Lane (Save/Load Game)
struct LaneSaveData {
    LaneType type = LaneType::ROAD;
    float yPosition = 0.f;
    int direction = 1;
    float speedMultiplier = 1.f;

    std::vector<float> spawnPattern;
    int patternIndex = 0;
    float spawnTimer = 0.f;

    std::vector<float> monsterPositionsX;
    std::vector<float> carpetPositionsX;
    std::vector<ItemData> items;
};

class Lane {
public:
    // Constructor 1: NEW GAME
    // spawnOrderIndex: thu tu sinh lane (0, 1, 2...) - dung de RAI THOI GIAN nha quai dau tien,
    // lane sinh truoc (index nho) se nha quai som hon lane sinh sau (index lon).
    Lane(LaneType type, float yPosition, int direction, float speedMultiplier, bool isEndlessMode, int spawnOrderIndex = 0);

    // Constructor 2: LOAD GAME (phuc hoi tu du lieu da luu)
    explicit Lane(const LaneSaveData& saveData);

    // Lane khong the copy (so huu unique_ptr<Sprite> + vector<Monster> khong the copy)
    // -> luon duoc quan ly qua std::unique_ptr<Lane> o cap tren (LaneManager).
    Lane(const Lane&) = delete;
    Lane& operator=(const Lane&) = delete;
    Lane(Lane&&) = default;
    Lane& operator=(Lane&&) = default;

    void update(float deltaTime, bool isRedLight);
    void render(sf::RenderWindow& window);

    LaneSaveData exportSaveData() const;

    float getYPosition() const;
    LaneType getType() const;

    // Can thiet cho CollisionSystem: cho phep "doc" danh sach quai ma khong sua duoc
    const std::vector<Monster>& getMonsters() const;
    const std::vector<Carpet>& getCarpets() const;
    int collectItemAt(float playerX);
    int collectItemWithMagnet(float playerX);
    void clearMonsters();

private:
    void setupWalkAssets();
    void setupFlyAssets();
    void setupRestAssets();
    void setupChasmAssets();

    LaneType type_ = LaneType::ROAD;
    float yPosition_ = 0.f;
    int direction_ = 1;
    float speedMultiplier_ = 1.f;
    float baseSpeed_ = 0.f;

    int monsterFrameCount_ = 1;
    float spawnTimer_ = 0.f;
    std::vector<float> spawnPattern_;
    int patternIndex_ = 0;

    std::unique_ptr<sf::Sprite> bgSprite_;
    // Con tro toi texture duoc cache boi ResourceManager - KHONG so huu, khong can giai phong
    const sf::Texture* monsterTexture_ = nullptr;
    const sf::Texture* carpetTexture_ = nullptr;

    std::vector<Monster> monsters_;
    std::vector<Carpet> carpets_;
    std::vector<ItemData> items_;
};
