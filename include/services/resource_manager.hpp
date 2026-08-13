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

    float getMusicVolume() const { return m_musicVolume_; }
    void setMusicVolume(float volume) { 
        m_musicVolume_ = std::clamp(volume, 0.f, 100.f); 
        // [GỌI HÀM CẬP NHẬT sf::Music CỦA BẠN Ở ĐÂY NẾU CÓ]
    }

    float getSfxVolume() const { return m_sfxVolume_; }
    void setSfxVolume(float volume) { 
        m_sfxVolume_ = std::clamp(volume, 0.f, 100.f); 
        // [GẬP NHẬT ÂM LƯỢNG CHO CÁC sf::Sound MÀ BẠN ĐANG PHÁT]
    }

private:
    ResourceManager() = default;

    std::unordered_map<std::string, sf::Texture> textureCache_; 
    std::unordered_map<std::string, sf::Font> fontCache_;      
    
    float m_musicVolume_ = 100.f;
    float m_sfxVolume_ = 100.f;
};