#pragma once

#include <vector>
#include "entities/lane.hpp"

// DTO: toan bo trang thai can luu cua 1 van game dang choi do.
// SaveGameRepository CHI lam viec voi DTO nay - khong dung truc tiep Lane/Player thuc.
struct GameSnapshot {
    int score = 0;
    int playerGridX = 0;
    int playerGridY = 0;
    std::vector<LaneSaveData> lanes;
};
