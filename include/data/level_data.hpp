#pragma once

#include <string>
#include <vector>

// DTO thuan du lieu, mo ta 1 man choi (Classic mode) doc tu file JSON.
// Khong phu thuoc vao bat ky entity thuc te nao -> de test, de serialize.
struct LevelData {
    std::string name;
    std::vector<std::vector<int>> mapMatrix;
};
