#include "services/level_repository.hpp"
#include <fstream>
#include <iostream>
#include <algorithm> // Để dùng std::find
#include "utils/json.hpp"
#include "core/config.hpp"

using json = nlohmann::json;

// ==========================================
// ĐỌC FILE LEVEL (Tương tự Load SaveGame)
// ==========================================
LevelData LevelRepository::loadLevel(const std::string& filepath) {
    LevelData data;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Loi: Khong the mo file JSON " << filepath << "\n";
        return data;
    }

    try {
        json j;
        file >> j;

        if (j.contains("name")) data.name = j["name"].get<std::string>();
        if (j.contains("collected_coins")) data.collectedCoins = j["collected_coins"].get<int>();
        if (j.contains("total_coins")) data.totalCoins = j["total_coins"].get<int>();

        // MỚI: Đọc mảng lanes y hệt như bên SaveGameRepository
        if (j.contains("lanes") && j["lanes"].is_array()) {
            for (const auto& j_lane : j["lanes"]) {
                LaneSaveData lData;
                
                lData.type = static_cast<LaneType>(j_lane[0].get<int>());
                lData.direction = j_lane[1].get<int>();
                lData.yPosition = j_lane[2].get<float>();
                lData.speedMultiplier = j_lane[3].get<float>();
                lData.spawnTimer = j_lane[4].get<float>();
                lData.patternIndex = j_lane[5].get<int>();

                int patternId = j_lane[6].get<int>();
                const auto& patternList = Config::SPAWN_PATTERNS;

                if (patternId >= 0 && patternId < patternList.size()) {
                    lData.spawnPattern = patternList[patternId];
                } else {
                    lData.spawnPattern = patternList[0];
                }

                lData.monsterPositionsX = j_lane[7].get<std::vector<float>>();
                
                // Đọc mảng Vàng/Item
                if (j_lane.size() > 8) {
                    for (const auto& j_item : j_lane[8]) {
                        ItemData item;
                        item.type = static_cast<ItemType>(j_item[0].get<int>());
                        item.x = j_item[1].get<float>();
                        lData.items.push_back(item);
                    }
                }
                
                // Đọc thảm bay
                if (j_lane.size() > 9) {
                    lData.carpetPositionsX = j_lane[9].get<std::vector<float>>();
                }

                data.lanes.push_back(lData);
            }
        }
    }
    catch (const json::exception& e) {
        std::cerr << "Loi parse JSON file level: " << e.what() << "\n";
    }

    file.close();
    return data;
}

// ==========================================
// GHI ĐÈ FILE LEVEL (Tương tự Save SaveGame)
// ==========================================
bool LevelRepository::saveLevel(const std::string& filepath, const LevelData& data) {
    json j_save;
    
    j_save["name"] = data.name;
    j_save["collected_coins"] = data.collectedCoins;
    j_save["total_coins"] = data.totalCoins;

    j_save["lanes"] = json::array();
    
    for (const auto& lData : data.lanes) {
        int patternId = 0;
        const auto& patternList = Config::SPAWN_PATTERNS;

        auto it = std::find(patternList.begin(), patternList.end(), lData.spawnPattern);
        if (it != patternList.end()) {
            patternId = std::distance(patternList.begin(), it);
        }

        json j_items = json::array();
        for (const auto& item : lData.items) {
            j_items.push_back({ static_cast<int>(item.type), item.x }); 
        }

        j_save["lanes"].push_back({
            static_cast<int>(lData.type),
            lData.direction,
            lData.yPosition,
            lData.speedMultiplier,
            lData.spawnTimer,
            lData.patternIndex,
            patternId,
            lData.monsterPositionsX,
            j_items,
            lData.carpetPositionsX
        });
    }

    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Loi: Khong the ghi de file JSON " << filepath << "\n";
        return false;
    }

    file << j_save.dump(4);
    file.close();
    return true;
}