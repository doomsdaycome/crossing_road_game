#include "gameplay/lane_manager.hpp"
#include "services/resource_manager.hpp"
#include <algorithm>
#include <cstdlib>

void LaneManager::initBackground(float startY) {
    // Lấy ảnh nền tổng
    const sf::Texture& texture = ResourceManager::instance().getTexture(Config::BG_TEXTURE);
    bgSprite1_ = std::make_unique<sf::Sprite>(texture);
    bgSprite2_ = std::make_unique<sf::Sprite>(texture);
    
    bgHeight_ = Config::VIEW_HEIGHT; // Chiều cao ảnh nhảy cóc

    // Đặt vị trí xuất phát
    bgSprite1_->setPosition({0.f, startY});
    bgSprite2_->setPosition({0.f, startY - bgHeight_});
}

void LaneManager::initEndless() {
    highestLaneY_ = Config::VIEW_HEIGHT;
    globalSpeedMultiplier_ = 1.0f;
    spawnOrderCounter_ = 0;
    consecutiveHazardCount_ = 0;

    // 2 lane dau tien ngay truoc mat nguoi choi la lane NGHI CHAN (khong quai),
    // cho nguoi choi vai giay lam quen truoc khi thuc su gap quai vat.
    int restCount = Config::REST_LANE_COUNT_AT_START;
    for (int i = 0; i < restCount; ++i) {
        spawnOneEndlessLane(true);
    }

    // MOI: sinh cac lane con lai bang pickNextLaneType() (thong qua spawnOneEndlessLane(false))
    // - tu day tro di, lane nghi chan se tiep tuc xuat hien XEN KE xuyen suot toan bo
    // qua trinh choi, khong con dung lai o vai lane dau game nua.
    int remaining = Config::ENDLESS_INITIAL_LANE_COUNT - restCount;
    for (int i = 0; i < remaining; ++i) {
        spawnOneEndlessLane(false);
    }
}

void LaneManager::buildFromSaveData(const std::vector<LaneSaveData>& savedLanes) {
    for (const auto& data : savedLanes) {
        lanes_.push_back(std::make_unique<Lane>(data));
        highestLaneY_ = std::min(highestLaneY_, data.yPosition);
    }
}

LaneType LaneManager::pickNextLaneType() {
    // Luat bat buoc: da qua nhieu lane nguy hiem lien tiep -> EP phai la lane nghi chan.
    // Day la phan quan trong nhat: dam bao man choi khong bao gio bi "dong" boi 1 chuoi
    // lane quai vo tan, giong nhu Crossy Road that luon co diem dung an toan dinh ky.
    if (consecutiveHazardCount_ >= Config::MAX_CONSECUTIVE_HAZARD_LANES) {
        consecutiveHazardCount_ = 0;
        return LaneType::REST;
    }

    // Luat ngau nhien: ngoai ra, moi lane van co 1 ti le nho la lane nghi chan
    // de nhip do khong bi "dem" qua may moc (du dung 3 lane la co nghi chan).
    int roll = rand() % 100;
    if (roll < Config::REST_LANE_CHANCE_PERCENT) {
        consecutiveHazardCount_ = 0;
        return LaneType::REST;
    }

    // Con lai: chia deu cho Road, Grass, va Chasm
    consecutiveHazardCount_++;
    int choice = rand() % 3;
    if (choice == 0) return LaneType::ROAD;
    if (choice == 1) return LaneType::GRASS;
    return LaneType::CHASM;
}

void LaneManager::spawnOneEndlessLane(bool forceRest) {
    highestLaneY_ -= Config::LANE_HEIGHT;

    LaneType type;
    if (forceRest) {
        type = LaneType::REST;
        consecutiveHazardCount_ = 0; // reset vi vua co 1 lane an toan
    } else {
        type = pickNextLaneType();
    }

    int randomDir = (rand() % 2 == 0) ? 1 : -1;

    // MOI: truyen thu tu sinh lane vao Lane de no tu rai thoi gian nha quai dau tien
    lanes_.push_back(std::make_unique<Lane>(type, highestLaneY_, randomDir, globalSpeedMultiplier_, true, spawnOrderCounter_));
    ++spawnOrderCounter_;
}

void LaneManager::update(float dt, bool isEndlessMode, float viewTopEdge, float viewBottomEdge, bool isRedLight) {
    if (bgSprite2_) {
        // Nếu mép trên camera vượt qua mép trên của tấm ảnh thứ 2
        if (viewTopEdge < bgSprite2_->getPosition().y) {
            // Nhấc tấm 1 ném lên trên đầu tấm 2
            bgSprite1_->setPosition({0.f, bgSprite2_->getPosition().y - bgHeight_});
            
            // Hoán đổi con trỏ (std::swap chạy cực nhanh với unique_ptr)
            std::swap(bgSprite1_, bgSprite2_);
        }
    }

    for (auto& lane : lanes_) {
        lane->update(dt, isRedLight);
    }

    if (isEndlessMode) {
        // MOI - KIEU FLAPPY BIRD: thay vi sinh 1 batch 5 lane cung luc khi vuot nguong,
        // dung vong lap "while" de sinh TUNG lane mot, moi lan cach nhau dung LANE_HEIGHT.
        // Giong het cach Flappy Bird luon giu 1 khoang cach co dinh phia truoc man hinh
        // truoc khi sinh ong tiep theo, bat ke frame rate/dt the nao.
        while (viewTopEdge < highestLaneY_ + Config::ENDLESS_GENERATE_THRESHOLD) {
            globalSpeedMultiplier_ += Config::ENDLESS_SPEED_INCREMENT;
            spawnOneEndlessLane(false);
        }

        lanes_.erase(
            std::remove_if(lanes_.begin(), lanes_.end(),
                [viewBottomEdge](const std::unique_ptr<Lane>& lane) {
                    return lane->getYPosition() > viewBottomEdge + Config::ENDLESS_LANE_CULL_MARGIN;
                }),
            lanes_.end()
        );
    }
}

void LaneManager::render(sf::RenderWindow& window) {
    if (bgSprite1_ && bgSprite2_) {
        window.draw(*bgSprite1_);
        window.draw(*bgSprite2_);
    }
    
    for (auto& lane : lanes_) {
        lane->render(window);
    }
}

std::vector<LaneSaveData> LaneManager::exportSaveData() const {
    std::vector<LaneSaveData> result;
    result.reserve(lanes_.size());
    for (const auto& lane : lanes_) {
        result.push_back(lane->exportSaveData());
    }
    return result;
}

const std::vector<std::unique_ptr<Lane>>& LaneManager::getLanes() const {
    return lanes_;
}

bool LaneManager::isChasmLane(float laneY) const {
    for (const auto& lane : lanes_) {
        if (std::abs(lane->getYPosition() - laneY) < 5.0f) {
            return lane->getType() == LaneType::CHASM;
        }
    }
    return false;
}

float LaneManager::getGlobalSpeedMultiplier() const {
    return globalSpeedMultiplier_;
}
