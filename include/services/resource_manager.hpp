#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // MỚI: Thêm thư viện Audio
#include <unordered_map>
#include <string>
#include <iostream>
#include <list>
#include <memory>
#include <algorithm>

class ResourceManager {
public:
    static ResourceManager& instance() {
        static ResourceManager manager;
        return manager;
    }

    // ==========================================
    // 1. QUẢN LÝ HÌNH ẢNH (TEXTURE)
    // ==========================================
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
    // 2. QUẢN LÝ FONT CHỮ
    // ==========================================
    const sf::Font& getFont(const std::string& filepath) {
        auto it = fontCache_.find(filepath);
        if (it != fontCache_.end()) {
            return it->second;
        }

        sf::Font font;
        if (!font.openFromFile(filepath)) {
            std::cerr << "Loi: Khong the load font: " << filepath << "\n";
        }

        auto [insertedIt, success] = fontCache_.emplace(filepath, std::move(font));
        (void)success;
        return insertedIt->second;
    }

    // ==========================================
    // 3. QUẢN LÝ ÂM THANH NGẮN (SFX - CACHE BUFFER)
    // ==========================================
    const sf::SoundBuffer& getSoundBuffer(const std::string& filepath) {
        auto it = soundBufferCache_.find(filepath);
        if (it != soundBufferCache_.end()) {
            return it->second;
        }

        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(filepath)) {
            std::cerr << "Loi: Khong the load sound: " << filepath << "\n";
        }

        auto [insertedIt, success] = soundBufferCache_.emplace(filepath, std::move(buffer));
        (void)success;
        return insertedIt->second;
    }

    void playSound(const std::string& filepath) {
        const sf::SoundBuffer& buffer = getSoundBuffer(filepath);
        
        // Dọn dẹp các sound đã phát xong để chống tràn RAM
        activeSounds_.remove_if([](const sf::Sound& s) {
            return s.getStatus() == sf::Sound::Status::Stopped; 
        });

        // Tạo sound mới, nạp buffer, áp dụng âm lượng hiện tại và phát
        activeSounds_.emplace_back(buffer);
        activeSounds_.back().setVolume(m_sfxVolume_);
        activeSounds_.back().play();
    }

    // ==========================================
    // 4. QUẢN LÝ NHẠC NỀN (MUSIC - STREAM TRỰC TIẾP)
    // ==========================================
    void playMusic(const std::string& filepath, bool loop = true) {
        // LỚP BẢO HIỂM: Nếu bài nhạc này ĐANG PHÁT rồi thì bỏ qua, cho nó hát tiếp
        if (currentMusicFilePath_ == filepath && currentMusic_ && currentMusic_->getStatus() == sf::SoundSource::Status::Playing) {
            return; 
        }

        if (!currentMusic_) {
            currentMusic_ = std::make_unique<sf::Music>();
        }

        if (!currentMusic_->openFromFile(filepath)) {
            std::cerr << "Loi: Khong the load music: " << filepath << "\n";
            return;
        }

        currentMusicFilePath_ = filepath; // Cập nhật tên bài mới
        currentMusic_->setVolume(m_musicVolume_);
        currentMusic_->setLooping(loop);
        currentMusic_->play();
    }

    void stopMusic() {
        if (currentMusic_) {
            currentMusic_->stop();
        }
    }

    // ==========================================
    // 5. ĐIỀU CHỈNH ÂM LƯỢNG (VOLUME CONTROL)
    // ==========================================
    float getMusicVolume() const { return m_musicVolume_; }
    void setMusicVolume(float volume) { 
        m_musicVolume_ = std::clamp(volume, 0.f, 100.f); 
        if (currentMusic_) {
            currentMusic_->setVolume(m_musicVolume_);
        }
    }

    float getSfxVolume() const { return m_sfxVolume_; }
    void setSfxVolume(float volume) { 
        m_sfxVolume_ = std::clamp(volume, 0.f, 100.f); 
        // Cập nhật âm lượng ngay lập tức cho TẤT CẢ các sound đang phát
        for (auto& sound : activeSounds_) {
            sound.setVolume(m_sfxVolume_);
        }
    }

    // ==========================================
    // 6. HỆ THỐNG DỌN DẸP
    // ==========================================
    void clear() {
        textureCache_.clear();
        fontCache_.clear(); 
        soundBufferCache_.clear();
        activeSounds_.clear();
        if (currentMusic_) {
            currentMusic_->stop();
        }
    }

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    ResourceManager() = default;

    std::unordered_map<std::string, sf::Texture> textureCache_; 
    std::unordered_map<std::string, sf::Font> fontCache_;      
    
    // Cache cho các tiếng động ngắn (Chứa cục data âm thanh)
    std::unordered_map<std::string, sf::SoundBuffer> soundBufferCache_; 
    
    // Danh sách các băng cassette (sf::Sound) đang cắm điện phát âm thanh
    // Dùng std::list thay vì vector để không bị lỗi con trỏ khi phần tử bị xóa
    std::list<sf::Sound> activeSounds_; 

    // Con trỏ quản lý duy nhất 1 luồng nhạc nền tại một thời điểm
    std::unique_ptr<sf::Music> currentMusic_; 
    
    float m_musicVolume_ = 100.f;
    float m_sfxVolume_ = 100.f;
    std::string currentMusicFilePath_ = "";
};