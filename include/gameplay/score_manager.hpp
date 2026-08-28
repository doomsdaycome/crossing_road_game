#pragma once

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>
#include <sstream>

inline std::string xorEncryptDecryptScore(const std::string& input) {
  std::string key = "CROSSING_ROAD_SECRET_2026";
  std::string output = input;
  for (size_t i = 0; i < input.size(); ++i) {
    output[i] = input[i] ^ key[i % key.length()];
  }
  return output;
}

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
      saveHighScore();  // Mua xong tự động lưu ngay lập tức
      return true;
    }
    return false;  // Không đủ tiền
  }

  // ĐỌC CẢ ĐIỂM, VÀNG, SKIN
  void loadHighScore() {
    std::ifstream file("data/highscore.txt", std::ios::binary);
    if (file.is_open()) {
      std::string fileData((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
      std::string parsedData = fileData;

      size_t firstNonWhitespace = fileData.find_first_not_of(" \t\n\r");
      if (firstNonWhitespace != std::string::npos &&
          (std::isdigit(fileData[firstNonWhitespace]) ||
           fileData[firstNonWhitespace] == '-')) {
        // File cũ chưa mã hóa
      } else {
        parsedData = xorEncryptDecryptScore(fileData);
      }

      std::stringstream ss(parsedData);

      ss >> bestScore_ >> totalCoins_;
      if (!(ss >> equippedSkin_ >> unlockedSkins_)) {
        equippedSkin_ = 0;
        unlockedSkins_ = 1;  // Bitmask: skin 0 is unlocked
      }
      if (!(ss >> magnetCount_ >> shieldCount_ >> invisCount_ >> novaCount_)) {
        magnetCount_ = 0;
        shieldCount_ = 0;
        invisCount_ = 0;
        novaCount_ = 0;
      }
      file.close();
    } else {
      bestScore_ = 0;
      totalCoins_ = 0;
      equippedSkin_ = 0;
      unlockedSkins_ = 1;
      magnetCount_ = 0;
      shieldCount_ = 0;
      invisCount_ = 0;
      novaCount_ = 0;
    }
  }

  // GHI CẢ ĐIỂM, VÀNG, SKIN
  void saveHighScore() const {
    std::ofstream file("data/highscore.txt", std::ios::binary);
    if (file.is_open()) {
      std::stringstream ss;
      ss << bestScore_ << " " << totalCoins_ << " " << equippedSkin_ << " "
         << unlockedSkins_ << " " << magnetCount_ << " " << shieldCount_ << " "
         << invisCount_ << " " << novaCount_;

      std::string rawData = ss.str();
      std::string encryptedData = xorEncryptDecryptScore(rawData);
      file.write(encryptedData.data(), encryptedData.size());
      file.close();
    }
  }

  // --- QUẢN LÝ SKIN ---
  int getEquippedSkin() const { return equippedSkin_; }
  void setEquippedSkin(int id) { equippedSkin_ = id; }

  bool isSkinUnlocked(int id) const {
    return (unlockedSkins_ & (1 << id)) != 0;
  }
  void unlockSkin(int id) { unlockedSkins_ |= (1 << id); }

  // --- QUẢN LÝ BUFFS ---
  int getMagnetCount() const { return magnetCount_; }
  void setMagnetCount(int c) { magnetCount_ = c; }
  void addMagnet() { magnetCount_++; }

  int getShieldCount() const { return shieldCount_; }
  void setShieldCount(int c) { shieldCount_ = c; }
  void addShield() { shieldCount_++; }

  int getInvisCount() const { return invisCount_; }
  void setInvisCount(int c) { invisCount_ = c; }
  void addInvis() { invisCount_++; }

  int getNovaCount() const { return novaCount_; }
  void setNovaCount(int c) { novaCount_ = c; }
  void addNova() { novaCount_++; }

 private:
  int score_ = 0;
  int bestScore_ = 0;
  int totalCoins_ = 0;
  int equippedSkin_ = 0;
  int unlockedSkins_ = 1;  // 0b0001 (Skin 0 is always unlocked)

  int magnetCount_ = 0;
  int shieldCount_ = 0;
  int invisCount_ = 0;
  int novaCount_ = 0;
};