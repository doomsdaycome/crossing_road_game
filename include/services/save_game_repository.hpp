#pragma once

#include <string>
#include <optional>
#include "data/game_snapshot.hpp"

// ==========================================
// SAVE GAME REPOSITORY
// Chi lam MOT viec: luu/tai trang thai game dang choi (runtime data).
// load() gio da tra ve du lieu that su (ban goc chi la stub in ra console).
// ==========================================
namespace SaveGameRepository {
    bool save(const std::string& filepath, const GameSnapshot& snapshot);
    std::optional<GameSnapshot> load(const std::string& filepath);
}
