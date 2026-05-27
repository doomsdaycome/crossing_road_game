#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <iostream>

class ResourceManager {
private:
    // Cấu trúc từ điển: Chìa khóa (Tên file) -> Giá trị (Bức ảnh)
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::SoundBuffer> m_soundpacks;

    // Chặn không cho tạo đối tượng bừa bãi (Singleton Pattern)
    ResourceManager() {} 

public:
    // Hàm lấy "nhà kho" duy nhất của game
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    // Xóa copy constructor để tránh lỡ tay nhân bản nhà kho
    ResourceManager(const ResourceManager&) = delete;
    void operator=(const ResourceManager&) = delete;

    sf::Texture& getTexture(const std::string& filename) {
        // Kiểm tra xem ảnh đã được load vào kho chưa?
        auto it = m_textures.find(filename);
        
        if (it != m_textures.end()) {
            // Đã load rồi -> Lấy ra dùng luôn, không đọc file nữa
            return it->second;
        } else {
            // Chưa load -> Bắt đầu đọc từ ổ cứng và lưu vào kho
            sf::Texture texture;
            if (!texture.loadFromFile(filename)) {
                std::cerr << "Loi: Khong the tai anh " << filename << std::endl;
            }
            m_textures[filename] = texture;
            return m_textures[filename];
        }
    }

    sf::SoundBuffer& getSoundPacks(const std::string& filename) {
        // Kiểm tra xem âm thanh đã được load vào kho chưa?
        auto it = m_soundpacks.find(filename);
        
        if (it != m_soundpacks.end()) {
            // Đã load rồi -> Lấy ra dùng luôn, không đọc file nữa
            return it->second;
        } else {
            // Chưa load -> Bắt đầu đọc từ ổ cứng và lưu vào kho
            sf::SoundBuffer buffers;
            if (!buffers.loadFromFile(filename)) {
                std::cerr << "Loi: Khong the tai am thanh " << filename << std::endl;
            }
            m_soundpacks[filename] = buffers;
            return m_soundpacks[filename];
        }
    }
};