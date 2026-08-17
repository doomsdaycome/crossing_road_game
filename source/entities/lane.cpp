#include "entities/lane.hpp"
#include "core/config.hpp"
#include "services/resource_manager.hpp"
#include "utils/utils.hpp"
#include <algorithm>
#include <cstdlib>

// ==========================================
// 1. CONSTRUCTOR: NEW GAME
// ==========================================
Lane::Lane(LaneType type, float yPosition, int direction, float speedMultiplier, bool isEndlessMode, int spawnOrderIndex)
    : type_(type), yPosition_(yPosition), direction_(direction), speedMultiplier_(speedMultiplier), 
      patternIndex_(0), spawnTimer_(0.f)
{
    // --- Khởi tạo tài nguyên theo loại Làn ---
    if (type_ == LaneType::REST) {
        setupRestAssets();
        spawnPattern_.clear();
    } 
    else {
        if (type_ == LaneType::ROAD) setupZombieAssets();
        else setupBatAssets();

        // Chọn kịch bản sinh quái ngẫu nhiên
        const auto& patterns = Config::SPAWN_PATTERNS;
        if (!patterns.empty()) {
            spawnPattern_ = patterns[static_cast<size_t>(rand()) % patterns.size()];
        } else {
            spawnPattern_ = {1.f};
        }

        // Tính độ trễ sinh quái đầu tiên (Stagger Delay) để tránh quái kẹt thành hàng dọc
        spawnTimer_ = static_cast<float>(spawnOrderIndex % Config::SPAWN_STAGGER_SLOTS) * Config::SPAWN_STAGGER_STEP;
    }

    if (bgSprite_) {
        bgSprite_->setPosition({0.f, yPosition_});
    }

    // --- Khởi tạo Vật phẩm ngẫu nhiên (Chỉ Endless) ---
    if (isEndlessMode && type_ != LaneType::REST) { 
        if (std::rand() % 100 < 35) { // 35% tỷ lệ xuất hiện vật phẩm
            ItemData newItem;
            
            // Đổ xí ngầu độ hiếm: 70% Xu(1), 25% Xu(3), 5% Rương(50)
            int rollRate = std::rand() % 100;
            if (rollRate < 70) newItem.type = ItemType::COIN_1;
            else if (rollRate < 95) newItem.type = ItemType::COIN_3;
            else newItem.type = ItemType::TREASURE;

            // ĐÃ SỬA LỖI CRASH CHIA CHO 0 BẰNG LỚP BẢO VỆ std::max
            int totalColumns = static_cast<int>(Config::WINDOW_WIDTH / Config::TILE_SIZE);
            int maxCol = std::max(1, totalColumns - 2); 
            int randomCol = 1 + std::rand() % maxCol; 
            
            newItem.x = static_cast<float>(randomCol * Config::TILE_SIZE);
            items_.push_back(newItem);
        }
    }
}

// ==========================================
// 2. CONSTRUCTOR: LOAD GAME (Khôi phục trạng thái)
// ==========================================
Lane::Lane(const LaneSaveData& saveData)
    : type_(saveData.type), yPosition_(saveData.yPosition), direction_(saveData.direction),
      speedMultiplier_(saveData.speedMultiplier), spawnPattern_(saveData.spawnPattern),
      patternIndex_(saveData.patternIndex), spawnTimer_(saveData.spawnTimer), items_(saveData.items)
{
    // --- Khôi phục tài nguyên ---
    if (type_ == LaneType::REST) setupRestAssets();
    else if (type_ == LaneType::ROAD) setupZombieAssets();
    else setupBatAssets();

    if (bgSprite_) {
        bgSprite_->setPosition({0.f, yPosition_});
    }

    // --- Khôi phục Quái vật trên làn ---
    float finalSpeed = baseSpeed_ * speedMultiplier_;
    
    // ĐÃ SỬA LỖI CRASH (Ngăn lỗi Null Pointer khi Load quái ở Làn nghỉ chân)
    if (monsterTexture_ != nullptr) {
        for (float posX : saveData.monsterPositionsX) {
            monsters_.emplace_back(*monsterTexture_, posX, yPosition_, finalSpeed, direction_, monsterFrameCount_);
        }
    }
}

// ==========================================
// 3. CÁC HÀM SETUP TÀI NGUYÊN (ASSETS)
// ==========================================
void Lane::setupZombieAssets() {
    const sf::Texture& bgTex = ResourceManager::instance().getTexture(Config::ZOMBIE_LANE_TEXTURE);
    monsterTexture_ = &ResourceManager::instance().getTexture(Config::ZOMBIE_TEXTURE);
    bgSprite_ = std::make_unique<sf::Sprite>(bgTex);
    baseSpeed_ = Config::ZOMBIE_BASE_SPEED;
    monsterFrameCount_ = Config::ZOMBIE_FRAME_COUNT;
}

void Lane::setupBatAssets() {
    const sf::Texture& bgTex = ResourceManager::instance().getTexture(Config::BAT_LANE_TEXTURE);
    monsterTexture_ = &ResourceManager::instance().getTexture(Config::BAT_TEXTURE);
    bgSprite_ = std::make_unique<sf::Sprite>(bgTex);
    baseSpeed_ = Config::BAT_BASE_SPEED;
    monsterFrameCount_ = Config::BAT_FRAME_COUNT;
}

void Lane::setupRestAssets() {
    // Làn nghỉ cố tình set bgSprite_ = nullptr để lộ nền tổng phía sau
    bgSprite_ = nullptr;
    baseSpeed_ = 0.f;
    monsterFrameCount_ = 1;
    monsterTexture_ = nullptr;
}

// ==========================================
// 4. XUẤT DỮ LIỆU ĐỂ LƯU GAME
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
    data.items = items_;

    for (const auto& monster : monsters_) {
        data.monsterPositionsX.push_back(monster.getPositionX());
    }
    return data;
}

// ==========================================
// 5. CẬP NHẬT LOGIC (Mỗi Frame)
// ==========================================
void Lane::update(float deltaTime, bool isRedLight) {
    if (type_ != LaneType::REST && !isRedLight) {
        spawnTimer_ -= deltaTime;

        if (spawnTimer_ <= 0.f && !spawnPattern_.empty()) {
            
            bool canSpawn = true;
            float spawnX = (direction_ == 1) ? Config::MONSTER_SPAWN_START_LEFT : Config::MONSTER_SPAWN_START_RIGHT;

            // Khóa khoảng cách (Physical Gap Lock): 
            // Bắt buộc cách đuôi con trước ít nhất 1 TILE + 20px
            if (!monsters_.empty()) {
                float distanceToLast = std::abs(monsters_.back().getPositionX() - spawnX);
                if (distanceToLast < Config::TILE_SIZE + 20.f) { 
                    canSpawn = false;
                }
            }

            if (canSpawn && monsterTexture_ != nullptr) {
                float finalSpeed = baseSpeed_ * speedMultiplier_;
                monsters_.emplace_back(*monsterTexture_, spawnX, yPosition_, finalSpeed, direction_, monsterFrameCount_);

                // Ép ngưỡng delay tối thiểu để quái không bị đẻ ra chồng chéo khi tốc độ quá cao
                float nextDelay = spawnPattern_[static_cast<size_t>(patternIndex_)];
                float delayReduction = 1.0f + (speedMultiplier_ - 1.0f) * 0.25f; 
                float actualDelay = std::max(0.5f, nextDelay / delayReduction);

                spawnTimer_ = actualDelay;
                patternIndex_ = (patternIndex_ + 1) % static_cast<int>(spawnPattern_.size());
            }
        }
    }

    for (auto& monster : monsters_) {
        monster.update(deltaTime, isRedLight);
    }

    // Dọn dẹp quái vật ra khỏi màn hình
    monsters_.erase(
        std::remove_if(monsters_.begin(), monsters_.end(), [](const Monster& m) { return m.isOffScreen(); }),
        monsters_.end()
    );
}

// ==========================================
// 6. RENDER VÀ KIỂM TRA VA CHẠM
// ==========================================
void Lane::render(sf::RenderWindow& window) {
    if (bgSprite_) {
        window.draw(*bgSprite_);
    }

    // TỐI ƯU HÓA RENDER VẬT PHẨM BẰNG STACK ALLOCATION
    if (!items_.empty()) {
        auto& rm = ResourceManager::instance();

        for (const auto& item : items_) {
            const sf::Texture* tex = nullptr;
            if (item.type == ItemType::COIN_1) tex = &rm.getTexture(Config::COIN_1_TEXTURE);
            else if (item.type == ItemType::COIN_3) tex = &rm.getTexture(Config::COIN_3_TEXTURE);
            else if (item.type == ItemType::TREASURE) tex = &rm.getTexture(Config::TREASURE_TEXTURE);

            if (tex != nullptr) {
                sf::Sprite spr(*tex); // Tạo thẳng trên Stack cực nhanh, tránh cấp phát bộ nhớ bừa bãi
                centerOrigin(spr);
                spr.setPosition({item.x + Config::TILE_SIZE / 2.f, yPosition_ + Config::TILE_SIZE / 2.f});
                window.draw(spr);
            }
        }
    }

    for (auto& monster : monsters_) {
        monster.render(window);
    }
}

int Lane::collectItemAt(float playerX) {
    for (auto it = items_.begin(); it != items_.end(); ) {
        // Va chạm khi khoảng cách < nửa ô Tile
        if (std::abs(it->x - playerX) < Config::TILE_SIZE / 2.0f) {
            int earned = 0;
            if (it->type == ItemType::COIN_1) earned = 1;
            else if (it->type == ItemType::COIN_3) earned = 3;
            else if (it->type == ItemType::TREASURE) earned = 50;

            it = items_.erase(it); // Xóa khỏi bộ nhớ, tự động mất hình
            return earned;
        } else {
            ++it;
        }
    }
    return 0; 
}

float Lane::getYPosition() const { return yPosition_; }
LaneType Lane::getType() const { return type_; }
const std::vector<Monster>& Lane::getMonsters() const { return monsters_; }