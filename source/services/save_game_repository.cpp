#include "services/save_game_repository.hpp"

#include <fstream>
#include <iostream>
#include <iterator>

#include "core/config.hpp"
#include "utils/json.hpp"

using json = nlohmann::json;

namespace {
std::string xorEncryptDecrypt(const std::string& input) {
  std::string key = "CROSSING_ROAD_SECRET_2026";
  std::string output = input;
  for (size_t i = 0; i < input.size(); ++i) {
    output[i] = input[i] ^ key[i % key.length()];
  }
  return output;
}
}  // namespace

bool SaveGameRepository::save(const std::string& filepath,
                              const GameSnapshot& snapshot) {
  json j_save;

  j_save["player"]["score"] = snapshot.score;
  j_save["player"]["grid_x"] = snapshot.playerGridX;
  j_save["player"]["grid_y"] = snapshot.playerGridY;

  j_save["lanes"] = json::array();
  for (const auto& lData : snapshot.lanes) {
    int patternId = 0;
    const auto& patternList = Config::SPAWN_PATTERNS;

    auto it =
        std::find(patternList.begin(), patternList.end(), lData.spawnPattern);
    if (it != patternList.end()) {
      patternId = std::distance(patternList.begin(), it);
    }

    json j_items = json::array();
    for (const auto& item : lData.items) {
      // Lưu dưới dạng mảng con [loại_vàng, tọa_độ_x]
      j_items.push_back({static_cast<int>(item.type), item.x});
    }

    // [type, direction, y_position, speed_multiplier, spawn_timer,
    // pattern_index, spawn_pattern, monsters_x]
    j_save["lanes"].push_back(
        {static_cast<int>(lData.type), lData.direction, lData.yPosition,
         lData.speedMultiplier, lData.spawnTimer, lData.patternIndex, patternId,
         lData.monsterPositionsX, j_items, lData.carpetPositionsX});
  }

  // TODO (CHANGE PATH/VALUE): dam bao thu muc cha cua filepath da ton tai truoc
  // khi ghi file
  std::ofstream file(filepath, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Loi: Khong the tao file save tai " << filepath << "\n";
    return false;
  }

  std::string rawJson = j_save.dump();
  std::string encryptedData = xorEncryptDecrypt(rawJson);
  file.write(encryptedData.data(), encryptedData.size());
  file.close();
  return true;
}

std::optional<GameSnapshot> SaveGameRepository::load(
    const std::string& filepath) {
  std::ifstream file(filepath, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Loi: Khong the mo file save " << filepath << "\n";
    return std::nullopt;
  }

  if (file.peek() == std::ifstream::traits_type::eof()) {
    return std::nullopt;
  }

  try {
    std::string fileData((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    std::string decryptedData = xorEncryptDecrypt(fileData);
    json j = json::parse(decryptedData);

    GameSnapshot snapshot;
    snapshot.score = j["player"]["score"].get<int>();
    snapshot.playerGridX = j["player"]["grid_x"].get<int>();
    snapshot.playerGridY = j["player"]["grid_y"].get<int>();

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
      if (j_lane.size() > 8) {
        for (const auto& j_item : j_lane[8]) {
          ItemData item;
          item.type = static_cast<ItemType>(j_item[0].get<int>());
          item.x = j_item[1].get<float>();
          lData.items.push_back(item);
        }
      }
      if (j_lane.size() > 9) {
        lData.carpetPositionsX = j_lane[9].get<std::vector<float>>();
      }

      snapshot.lanes.push_back(lData);
    }

    std::cout << "Doc file save thanh cong!\n";
    return snapshot;
  } catch (const json::exception& e) {
    std::cerr << "Loi parse JSON file save: " << e.what() << "\n";
    return std::nullopt;
  }
}
