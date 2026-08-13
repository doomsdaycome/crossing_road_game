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

    // ĐỌC CẢ ĐIỂM VÀ VÀNG
    void loadHighScore() {
        std::ifstream file("data/highscore.txt");
        if (file.is_open()) {
            file >> bestScore_ >> totalCoins_; // Đọc 2 biến cách nhau bởi dấu space hoặc xuống dòng
            file.close();
        } else {
            bestScore_ = 0; 
            totalCoins_ = 0; // Chưa chơi thì vàng = 0
        }
    }

    // GHI CẢ ĐIỂM VÀ VÀNG
    void saveHighScore() const {
        std::ofstream file("data/highscore.txt");
        if (file.is_open()) {
            file << bestScore_ << " " << totalCoins_; // Lưu 2 số cách nhau 1 khoảng trắng
            file.close();
        }
    }

private:
    int score_ = 0;
    int bestScore_ = 0;
    int totalCoins_ = 0;
};