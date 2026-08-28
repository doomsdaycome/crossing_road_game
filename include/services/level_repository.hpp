#pragma once

#include <string>

#include "data/level_data.hpp"

// ==========================================
// LEVEL REPOSITORY
// Chi lam MOT viec: doc dinh nghia level (design-time data) tu JSON.
// Tach khoi SaveGameRepository (runtime data) - fix SRP violation cua
// DataManager cu.
// ==========================================

class LevelRepository {
 public:
  static LevelData loadLevel(const std::string& filepath);
  static bool saveLevel(const std::string& filepath, const LevelData& data);

 private:
  LevelRepository() = delete;
};