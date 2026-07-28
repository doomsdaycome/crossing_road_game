#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "entities/monster.hpp"

// Loai lane. enum class de de mo rong (them WATER, RAIL...) sau nay
// ma khong phai sua if/else rai rac khap noi (fix OCP violation trong audit).
enum class LaneType {
    ROAD = 0,
    GRASS = 1
};

// DTO dung de luu/khoi phuc trang thai 1 Lane (Save/Load Game)
struct LaneSaveData {
    LaneType type = LaneType::ROAD;
    float yPosition = 0.f;
    int direction = 1;
    float speedMultiplier = 1.f;

    std::vector<int> spawnPattern;
    int patternIndex = 0;
    float spawnTimer = 0.f;

    std::vector<float> monsterPositionsX;
};

class Lane {
private:
    void setupRoadAssets();
    void setupGrassAssets();

    LaneType type_ = LaneType::ROAD;
    float yPosition_ = 0.f;
    int direction_ = 1;
    float speedMultiplier_ = 1.f;
    float baseSpeed_ = 0.f;

    int monsterFrameCount_ = 1;
    float spawnInterval_ = 1.f;
    float spawnTimer_ = 0.f;
    std::vector<int> spawnPattern_;
    int patternIndex_ = 0;

    std::unique_ptr<sf::Sprite> bgSprite_;
    // Con tro toi texture duoc cache boi ResourceManager - KHONG so huu, khong can giai phong
    const sf::Texture* monsterTexture_ = nullptr;

    std::vector<Monster> monsters_;
    
public:
    // Constructor 1: NEW GAME
    Lane(LaneType type, float yPosition, int direction, float speedMultiplier, bool isEndlessMode);

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
};
