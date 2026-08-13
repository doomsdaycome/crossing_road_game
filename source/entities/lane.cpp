#include "entities/lane.hpp"
#include "core/config.hpp"
#include "services/resource_manager.hpp"
#include "utils/utils.hpp"
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
            setupZombieAssets();
        } else {
            setupBatAssets();
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
        spawnTimer_ = staggerDelay;
    }
    if (bgSprite_) {
        bgSprite_->setPosition({0.f, yPosition_});
    }

    if (isEndlessMode && type_ != LaneType::REST) { 
        
        // Tỷ lệ xuất hiện: 35% cơ hội làn này sẽ đẻ ra vàng
        if (std::rand() % 100 < 35) {
            ItemData newItem;
            
            // Tỷ lệ độ hiếm: 70% Xu đơn (1đ), 25% Xu ba (3đ), 5% Rương (50đ)
            int rollRate = std::rand() % 100;
            if (rollRate < 70) {
                newItem.type = ItemType::COIN_1;
            } else if (rollRate < 95) {
                newItem.type = ItemType::COIN_3;
            } else {
                newItem.type = ItemType::TREASURE;
            }

            // Random tọa độ X theo đúng chuẩn Lưới (Grid) để vàng nằm ngay ngắn giữa ô
            // Giả sử lấy chiều rộng màn hình chia cho kích thước Tile để ra số cột
            int totalColumns = static_cast<int>(Config::WINDOW_WIDTH / Config::TILE_SIZE);
            
            // Chừa lại 1 ô ở hai bên mép màn hình để vàng không bị rớt ra ngoài lề
            int randomCol = 1 + std::rand() % (totalColumns - 2); 
            
            // Gán tọa độ X (Tọa độ Y thì tự động ăn theo yPosition_ của Làn lúc render rồi)
            newItem.x = static_cast<float>(randomCol * Config::TILE_SIZE);

            items_.push_back(newItem);
        }
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
        setupZombieAssets();
    } else {
        setupBatAssets();
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

    items_ = saveData.items;
}

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
    // MOI: lane nghi chan - chi co nen, KHONG co quai nen khong can load monsterTexture_
    const sf::Texture& bgTex = ResourceManager::instance().getTexture(Config::REST_LANE_TEXTURE);

    bgSprite_ = nullptr;
    baseSpeed_ = 0.f;
    monsterFrameCount_ = 1;
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

    data.items = items_;

    return data;
}

// ==========================================
// LOGIC CAP NHAT (NHA QUAI)
// ==========================================
void Lane::update(float deltaTime, bool isRedLight) {
    // MOI: lane REST (nghi chan) khong bao gio nha quai
    if (type_ != LaneType::REST && !isRedLight) {
        spawnTimer_ -= deltaTime;

        if (spawnTimer_ <= 0.f && !spawnPattern_.empty()) {
            
            // ==========================================
            // 1. LỚP BẢO HIỂM KHOẢNG CÁCH (Physical Gap Lock)
            // ==========================================
            bool canSpawn = true;
            if (!monsters_.empty()) {
                // Lấy tọa độ của con quái vật vừa sinh ra gần nhất
                const auto& lastMonster = monsters_.back();
                float spawnX = (direction_ == 1) ? Config::MONSTER_SPAWN_START_LEFT : Config::MONSTER_SPAWN_START_RIGHT;
                
                // Đo khoảng cách vật lý từ chỗ sinh đến đuôi con quái
                float distanceToLast = std::abs(lastMonster.getPositionX() - spawnX);
                
                // BẮT BUỘC phải hở một khoảng bằng 1 TILE_SIZE (64px) + 20px lề thì mới cho sinh tiếp
                if (distanceToLast < Config::TILE_SIZE + 20.f) { 
                    canSpawn = false;
                }
            }

            // ==========================================
            // 2. CHỈ SINH KHI ĐÃ ĐỦ ĐIỀU KIỆN AN TOÀN
            // ==========================================
            if (canSpawn) {
                float startX = (direction_ == 1) ? Config::MONSTER_SPAWN_START_LEFT : Config::MONSTER_SPAWN_START_RIGHT;
                float finalSpeed = baseSpeed_ * speedMultiplier_;

                monsters_.push_back(Monster(*monsterTexture_, startX, yPosition_, finalSpeed, direction_, monsterFrameCount_));

                float nextDelay = spawnPattern_[static_cast<size_t>(patternIndex_)];
                
                // Tính toán delay và ÉP MỨC TỐI THIỂU (Clamp)
                // Ngăn chặn việc delay tụt xuống quá thấp khi speedMultiplier_ quá lớn
                float delayReduction = 1.0f + (speedMultiplier_ - 1.0f) * 0.25f; 
                float actualDelay = nextDelay / delayReduction;

                if (actualDelay < 0.5f) {
                    actualDelay = 0.5f; // Ép delay thấp nhất là 0.2 giây
                }

                spawnTimer_ = actualDelay;
                patternIndex_ = (patternIndex_ + 1) % static_cast<int>(spawnPattern_.size());
            }
            // Lưu ý: Nếu canSpawn == false, spawnTimer_ vẫn đang <= 0.
            // Tới frame tiếp theo, game sẽ nhảy vào kiểm tra khoảng cách lại cho đến khi đủ khoảng hở!
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

    const sf::Texture& coin1Tex = ResourceManager::instance().getTexture(Config::COIN_1_TEXTURE);
    const sf::Texture& coin3Tex = ResourceManager::instance().getTexture(Config::COIN_3_TEXTURE);
    const sf::Texture& chestTex = ResourceManager::instance().getTexture(Config::TREASURE_TEXTURE);

    std::unique_ptr<sf::Sprite> itemSprite;

    for (const auto& item : items_) {
        // Chọn Texture tương ứng với loại Vàng
        if (item.type == ItemType::COIN_1) {
            itemSprite = std::make_unique<sf::Sprite>(coin1Tex);
        } else if (item.type == ItemType::COIN_3) {
            itemSprite = std::make_unique<sf::Sprite>(coin3Tex);
        } else if (item.type == ItemType::TREASURE) {
            itemSprite = std::make_unique<sf::Sprite>(chestTex);
        }

        // Căn giữa Sprite để đặt tọa độ cho dễ
        centerOrigin(*(itemSprite));

        // Đặt tọa độ (item.x là hoành độ đã lưu, m_yPosition_ là tung độ của Làn)
        // Cộng thêm nửa TILE_SIZE để Vàng nằm lọt thỏm chính giữa ô vuông của Làn
        itemSprite->setPosition({item.x + Config::TILE_SIZE / 2.f, yPosition_ + Config::TILE_SIZE / 2.f});

        window.draw(*(itemSprite));
    }

    for (auto& monster : monsters_) {
        monster.render(window);
    }
}

float Lane::getYPosition() const { return yPosition_; }
LaneType Lane::getType() const { return type_; }
const std::vector<Monster>& Lane::getMonsters() const { return monsters_; }

int Lane::collectItemAt(float playerX) {
    for (auto it = items_.begin(); it != items_.end(); ) {
        // Kiểm tra khoảng cách: Nếu nhân vật đứng gần cục vàng (trong phạm vi nửa ô)
        if (std::abs(it->x - playerX) < Config::TILE_SIZE / 2.0f) {
            int earned = 0;
            if (it->type == ItemType::COIN_1) earned = 1;
            else if (it->type == ItemType::COIN_3) earned = 3;
            else if (it->type == ItemType::TREASURE) earned = 50;

            // Xóa ngay lập tức khỏi mảng -> Tự động biến mất khỏi màn hình ở frame sau!
            it = items_.erase(it); 
            
            // Có thể thêm âm thanh ting ting ở đây
            // ResourceManager::instance().playSound("coin_sfx");
            
            return earned; // Trả về số tiền ăn được
        } else {
            ++it;
        }
    }
    return 0; // Không chạm cục nào
}