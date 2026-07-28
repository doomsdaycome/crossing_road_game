#pragma once

#include <string>
#include "data/level_data.hpp"

// ==========================================
// LEVEL REPOSITORY
// Chi lam MOT viec: doc dinh nghia level (design-time data) tu JSON.
// Tach khoi SaveGameRepository (runtime data) - fix SRP violation cua DataManager cu.
// ==========================================
namespace LevelRepository {
    LevelData loadLevel(const std::string& filepath);
}
