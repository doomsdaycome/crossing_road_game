#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>

// ==========================================
// RESOURCE MANAGER (Singleton)
// Dam bao moi texture chi duoc load tu o dia DUY NHAT MOT LAN.
// Toan bo Entity (Lane, Monster, Player...) phai xin texture qua day,
// KHONG duoc tu goi loadFromFile() nua.
//
// Vi texture duoc giu song suot vong doi chuong trinh (static cache),
// cac sf::Sprite giu con tro/tham chieu toi texture nay se KHONG BAO GIO
// bi dangling - giai quyet triet de bug "SFML texture lifetime" da neu trong audit.
// ==========================================
class ResourceManager {
public:
    static ResourceManager& instance() {
        static ResourceManager manager;
        return manager;
    }

    // Tra ve tham chieu texture da cache. Neu chua co, load 1 lan roi cache lai.
    const sf::Texture& getTexture(const std::string& filepath) {
        auto it = cache_.find(filepath);
        if (it != cache_.end()) {
            return it->second;
        }

        sf::Texture texture;
        if (!texture.loadFromFile(filepath)) {
            std::cerr << "Loi: Khong the load texture: " << filepath << "\n";
        }

        auto [insertedIt, success] = cache_.emplace(filepath, std::move(texture));
        (void)success;
        return insertedIt->second;
    }

    // Xoa toan bo cache (dung khi chuyen man hinh lon / thoat game)
    void clear() {
        cache_.clear();
    }

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    ResourceManager() = default;
    std::unordered_map<std::string, sf::Texture> cache_;
};
