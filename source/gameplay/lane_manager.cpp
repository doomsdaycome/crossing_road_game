#include "gameplay/lane_manager.hpp"
#include <algorithm>
#include <cstdlib>

void LaneManager::buildFromLevel(const LevelData& levelData) {
    float currentY = Config::VIEW_HEIGHT - Config::LANE_HEIGHT;
    for (const auto& row : levelData.mapMatrix) {
        if (row.empty()) continue;

        LaneType type = (row[0] == 0) ? LaneType::ROAD : LaneType::GRASS;
        int randomDir = (rand() % 2 == 0) ? 1 : -1;

        lanes_.push_back(std::make_unique<Lane>(type, currentY, randomDir, 1.0f, false));
        currentY -= Config::LANE_HEIGHT;
    }
}

void LaneManager::initEndless() {
    highestLaneY_ = Config::VIEW_HEIGHT;
    globalSpeedMultiplier_ = 1.0f;
    generateEndlessLanes(Config::ENDLESS_INITIAL_LANE_COUNT);
}

void LaneManager::buildFromSaveData(const std::vector<LaneSaveData>& savedLanes) {
    for (const auto& data : savedLanes) {
        lanes_.push_back(std::make_unique<Lane>(data));
        highestLaneY_ = std::min(highestLaneY_, data.yPosition);
    }
}

void LaneManager::generateEndlessLanes(int count) {
    for (int i = 0; i < count; ++i) {
        highestLaneY_ -= Config::LANE_HEIGHT;
        LaneType randomType = (rand() % 2 == 0) ? LaneType::ROAD : LaneType::GRASS;
        int randomDir = (rand() % 2 == 0) ? 1 : -1;

        lanes_.push_back(std::make_unique<Lane>(randomType, highestLaneY_, randomDir, globalSpeedMultiplier_, true));
    }
}

void LaneManager::update(float dt, bool isEndlessMode, float viewTopEdge, float viewBottomEdge) {
    for (auto& lane : lanes_) {
        lane->update(dt, false);
    }

    if (isEndlessMode) {
        if (viewTopEdge < highestLaneY_ + Config::ENDLESS_GENERATE_THRESHOLD) {
            globalSpeedMultiplier_ += Config::ENDLESS_SPEED_INCREMENT;
            generateEndlessLanes(Config::ENDLESS_GENERATE_BATCH);
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

float LaneManager::getGlobalSpeedMultiplier() const {
    return globalSpeedMultiplier_;
}
