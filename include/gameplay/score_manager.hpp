#pragma once
#include <fstream>
#include <algorithm>

// ==========================================
// SCORE MANAGER (ĐÃ NÂNG CẤP KỶ LỤC)
// ==========================================
class ScoreManager {
public:
    // Đã đổi tên hàm theo đúng class của ông
    void addPoints(int points) { 
        score_ += points; 
        if (score_ > bestScore_) {
            bestScore_ = score_;
        }
    }
    
    int getScore() const { return score_; }
    int getBestScore() const { return bestScore_; }
    
    void reset() { score_ = 0; }
    void loadScore(int score) { score_ = score; }

    int getTotalCoins() const { return totalCoins_; }
    void addTotalCoins(int coins) { totalCoins_ += coins; }
    
    bool spendCoins(int price) {
        if (totalCoins_ >= price) {
            totalCoins_ -= price;
            saveHighScore(); // Mua xong tự động lưu ngay lập tức
            return true;
        }
        return false; // Không đủ tiền
    }

    // ĐỌC CẢ ĐIỂM, VÀNG, SKIN
    void loadHighScore() {
        std::ifstream file("data/highscore.txt");
        if (file.is_open()) {
            file >> bestScore_ >> totalCoins_;
            if (!(file >> equippedSkin_ >> unlockedSkins_)) {
                equippedSkin_ = 0;
                unlockedSkins_ = 1; // Bitmask: skin 0 is unlocked
            }
            file.close();
        } else {
            bestScore_ = 0; 
            totalCoins_ = 0;
            equippedSkin_ = 0;
            unlockedSkins_ = 1;
        }
    }

    // GHI CẢ ĐIỂM, VÀNG, SKIN
    void saveHighScore() const {
        std::ofstream file("data/highscore.txt");
        if (file.is_open()) {
            file << bestScore_ << " " << totalCoins_ << " " << equippedSkin_ << " " << unlockedSkins_;
            file.close();
        }
    }

    // --- QUẢN LÝ SKIN ---
    int getEquippedSkin() const { return equippedSkin_; }
    void setEquippedSkin(int id) { equippedSkin_ = id; }
    
    bool isSkinUnlocked(int id) const {
        return (unlockedSkins_ & (1 << id)) != 0;
    }
    void unlockSkin(int id) {
        unlockedSkins_ |= (1 << id);
    }

private:
    int score_ = 0;
    int bestScore_ = 0;
    int totalCoins_ = 0;
    int equippedSkin_ = 0;
    int unlockedSkins_ = 1; // 0b0001 (Skin 0 is always unlocked)
};