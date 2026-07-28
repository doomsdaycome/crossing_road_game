#include "services/level_repository.hpp"
#include <fstream>
#include <iostream>
#include "utils/json.hpp"

using json = nlohmann::json;

namespace LevelRepository {

LevelData loadLevel(const std::string& filepath) {
    LevelData data;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Loi: Khong the mo file JSON " << filepath << "\n";
        return data;
    }

    try {
        json j;
        file >> j;

        if (j.contains("name")) {
            data.name = j["name"].get<std::string>();
        }

        if (j.contains("map") && j["map"].is_array()) {
            data.mapMatrix = j["map"].get<std::vector<std::vector<int>>>();
        }
    }
    catch (const json::exception& e) {
        std::cerr << "Loi parse JSON file level: " << e.what() << "\n";
    }

    file.close();
    return data;
}

}
