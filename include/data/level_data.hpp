#pragma once

#include <string>
#include <vector>

#include "entities/lane.hpp"

// DTO thuan du lieu, mo ta 1 man choi (Classic mode) doc tu file JSON.
// Khong phu thuoc vao bat ky entity thuc te nao -> de test, de serialize.
struct LevelData {
  std::string name;
  std::vector<LaneSaveData> lanes;

  int collectedCoins = 0;
  int totalCoins = 0;
};
