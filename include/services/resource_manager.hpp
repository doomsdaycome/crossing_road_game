#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>

class ResourceManager {
public:
    static ResourceManager& instance() {
        static ResourceManager manager;
        return manager;
    }

    const sf::Texture& getTexture(const std::string& filepath) {
        auto it = textureCache_.find(filepath);
        if (it != textureCache_.end()) {
            return it->second;
        }

        sf::Texture texture;
        if (!texture.loadFromFile(filepath)) {
            std::cerr << "Loi: Khong the load texture: " << filepath << "\n";
        }

        auto [insertedIt, success] = textureCache_.emplace(filepath, std::move(texture));
        (void)success;
        return insertedIt->second;
    }

    // ==========================================
    // MỚI: Hàm Load và Cache Font chữ
    // ==========================================
    const sf::Font& getFont(const std::string& filepath) {
        auto it = fontCache_.find(filepath);
        if (it != fontCache_.end()) {
            return it->second;
        }

        sf::Font font;
        // LƯU Ý: SFML 3.x đổi hàm loadFromFile thành openFromFile đối với Font
        if (!font.openFromFile(filepath)) {
            std::cerr << "Loi: Khong the load font: " << filepath << "\n";
        }

        auto [insertedIt, success] = fontCache_.emplace(filepath, std::move(font));
        (void)success;
        return insertedIt->second;
    }

    void clear() {
        textureCache_.clear();
        fontCache_.clear(); // Nhớ clear cả cache font
    }

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    ResourceManager() = default;
    
    // Đổi tên biến cũ cho rõ nghĩa hơn
    std::unordered_map<std::string, sf::Texture> textureCache_; 
    
    // MỚI: Kho chứa Font
    std::unordered_map<std::string, sf::Font> fontCache_;       
};