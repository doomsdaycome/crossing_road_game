#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

class ResourceManager {
private:
    // Cấu trúc từ điển: Chìa khóa (Tên file) -> Giá trị (Bức ảnh)
    std::map<std::string, sf::Texture> m_textures;

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

    // Hàm cốt lõi: Yêu cầu lấy ảnh
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
};