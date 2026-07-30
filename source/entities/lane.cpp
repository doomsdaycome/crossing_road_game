#include "entities/lane.hpp"
#include "core/config.hpp"
#include "services/resource_manager.hpp"
#include <algorithm>
#include <cstdlib>

// ==========================================
// CONSTRUCTOR 1: NEW GAME
// ==========================================
Lane::Lane(LaneType type, float yPosition, int direction, float speedMultiplier, bool isEndlessMode, int spawnOrderIndex)
    : type_(type), yPosition_(yPosition), direction_(direction), speedMultiplier_(speedMultiplier)
{
    patternIndex_ = 0;
    spawnTimer_ = 0.f;
    (void)isEndlessMode; // Giu tham so de mo rong sau nay (VD: do kho rieng cho Endless)

    if (type_ == LaneType::REST) {
        // MOI: lane nghi chan - khong load texture quai, khong chon pattern nha quai
        setupRestAssets();
        spawnPattern_.clear();
    }
    else {
        if (type_ == LaneType::ROAD) {
            setupRoadAssets();
        } else {
            setupGrassAssets();
        }

        // FIX: ban goc co 2 nhanh if/else (isEndlessMode true/false) nhung lam y het nhau (dead code).
        // Gio chi con 1 duong duy nhat: chon ngau nhien 1 pattern tu Config theo loai lane.
        const auto& patterns = Config::SPAWN_PATTERNS;
        if (!patterns.empty()) {
            spawnPattern_ = patterns[static_cast<size_t>(rand()) % patterns.size()];
        } else {
            spawnPattern_ = {1};
        }

        // MOI: RAI THOI GIAN nha quai dau tien theo thu tu sinh lane - giong cach Flappy Bird
        // giu khoang cach deu giua cac ong. Lane sinh truoc (spawnOrderIndex nho) se dat
        // spawnTimer_ gan/bang spawnInterval_ -> nha quai gan nhu ngay lap tuc ("lam luon").
        // Lane sinh sau se co spawnTimer_ nho hon (co the am) -> phai cho lau hon moi nha quai.
        int staggerSlot = spawnOrderIndex % Config::SPAWN_STAGGER_SLOTS;
        float staggerDelay = static_cast<float>(staggerSlot) * Config::SPAWN_STAGGER_STEP;
        spawnTimer_ = spawnInterval_ - staggerDelay;
    }
    if (bgSprite_) {
        bgSprite_->setPosition({0.f, yPosition_});

    }
}

// ==========================================
// CONSTRUCTOR 2: LOAD GAME (PHUC HOI)
// ==========================================
Lane::Lane(const LaneSaveData& saveData)
    : type_(saveData.type),
      yPosition_(saveData.yPosition),
      direction_(saveData.direction),
      speedMultiplier_(saveData.speedMultiplier)
{
    spawnPattern_ = saveData.spawnPattern;
    patternIndex_ = saveData.patternIndex;
    spawnTimer_ = saveData.spawnTimer;

    // FIX: constructor nay truoc do thieu nhanh REST, khien lane REST khi Load Game
    // bi nham thanh GRASS (setupGrassAssets). Gio xu ly du ca 3 loai.
    if (type_ == LaneType::REST) {
        setupRestAssets();
    } else if (type_ == LaneType::ROAD) {
        setupRoadAssets();
    } else {
        setupGrassAssets();
    }

    if (bgSprite_) {
        bgSprite_->setPosition({0.f, yPosition_});

    }

    // FIX: ban goc dung 2 bo asset khac nhau giua "new game" va "load game"
    // (vd ln0.png vs road_asset.png) -> gio dung chung 1 nguon setupXAssets() nen luon dong bo.
    // Luu y: lane REST luon co monsterPositionsX rong (khong bao gio nha quai) nen vong lap
    // duoi day tu dong khong chay gi ca - an toan du monsterTexture_ dang la nullptr.
    float finalSpeed = baseSpeed_ * speedMultiplier_;

    for (float posX : saveData.monsterPositionsX) {
        monsters_.push_back(Monster(*monsterTexture_, posX, yPosition_, finalSpeed, direction_, monsterFrameCount_));
    }
}

void Lane::setupRoadAssets() {
    const sf::Texture& bgTex = ResourceManager::instance().getTexture(Config::ROAD_BG_TEXTURE);
    monsterTexture_ = &ResourceManager::instance().getTexture(Config::ROAD_MONSTER_TEXTURE);

    bgSprite_ = std::make_unique<sf::Sprite>(bgTex);
    baseSpeed_ = Config::ROAD_BASE_SPEED;
    monsterFrameCount_ = Config::ROAD_MONSTER_FRAME_COUNT;
    spawnInterval_ = Config::ROAD_SPAWN_INTERVAL;
}

void Lane::setupGrassAssets() {
    const sf::Texture& bgTex = ResourceManager::instance().getTexture(Config::GRASS_BG_TEXTURE);
    monsterTexture_ = &ResourceManager::instance().getTexture(Config::GRASS_MONSTER_TEXTURE);

    bgSprite_ = std::make_unique<sf::Sprite>(bgTex);
    baseSpeed_ = Config::GRASS_BASE_SPEED;
    monsterFrameCount_ = Config::GRASS_MONSTER_FRAME_COUNT;
    spawnInterval_ = Config::GRASS_SPAWN_INTERVAL;
}

void Lane::setupRestAssets() {
    // MOI: lane nghi chan - chi co nen, KHONG co quai nen khong can load monsterTexture_
    const sf::Texture& bgTex = ResourceManager::instance().getTexture(Config::REST_BG_TEXTURE);

    bgSprite_ = nullptr;
    baseSpeed_ = 0.f;
    monsterFrameCount_ = 1;
    spawnInterval_ = 0.f; // khong dung den vi update() se bo qua toan bo logic nha quai
    monsterTexture_ = nullptr;
}

// ==========================================
// XUAT DU LIEU (DE NEM CHO SAVE GAME REPOSITORY)
// ==========================================
LaneSaveData Lane::exportSaveData() const {
    LaneSaveData data;
    data.type = type_;
    data.yPosition = yPosition_;
    data.direction = direction_;
    data.speedMultiplier = speedMultiplier_;

    data.spawnPattern = spawnPattern_;
    data.patternIndex = patternIndex_;
    data.spawnTimer = spawnTimer_;

    for (const auto& monster : monsters_) {
        data.monsterPositionsX.push_back(monster.getPositionX());
    }

    return data;
}

// ==========================================
// LOGIC CAP NHAT (NHA QUAI)
// ==========================================
void Lane::update(float deltaTime, bool isRedLight) {
    // MOI: lane REST (nghi chan) khong bao gio nha quai
    if (type_ != LaneType::REST && !isRedLight) {
        spawnTimer_ += deltaTime;

        if (spawnTimer_ >= spawnInterval_ && !spawnPattern_.empty()) {
            // FIX: ban goc gan spawnTimer_ = spawnInterval_ (khong reset ve 0), co the lam
            // sai lech thoi gian nha quai neu dt lon (frame spike). Gio reset ve 0.
            spawnTimer_ = 0.f;

            int spawnCount = spawnPattern_[static_cast<size_t>(patternIndex_)];
            patternIndex_ = (patternIndex_ + 1) % static_cast<int>(spawnPattern_.size());

            float startX = (direction_ == 1) ? Config::MONSTER_SPAWN_START_LEFT : Config::MONSTER_SPAWN_START_RIGHT;
            float finalSpeed = baseSpeed_ * speedMultiplier_;

            for (int i = 0; i < spawnCount; ++i) {
                float offsetX = (direction_ == 1) ? -(i * Config::MONSTER_SPAWN_OFFSET_X) : (i * Config::MONSTER_SPAWN_OFFSET_X);
                monsters_.push_back(Monster(*monsterTexture_, startX + offsetX, yPosition_, finalSpeed, direction_, monsterFrameCount_));
            }
        }
    }

    for (auto& monster : monsters_) {
        monster.update(deltaTime, isRedLight);
    }

    // Don rac
    monsters_.erase(
        std::remove_if(monsters_.begin(), monsters_.end(),
            [](const Monster& m) { return m.isOffScreen(); }),
        monsters_.end()
    );
}

void Lane::render(sf::RenderWindow& window) {
    if (bgSprite_) {
        window.draw(*bgSprite_);
    }
    for (auto& monster : monsters_) {
        monster.render(window);
    }
}

float Lane::getYPosition() const { return yPosition_; }
LaneType Lane::getType() const { return type_; }
const std::vector<Monster>& Lane::getMonsters() const { return monsters_; }
