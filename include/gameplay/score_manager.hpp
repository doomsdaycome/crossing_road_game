#pragma once

// ==========================================
// SCORE MANAGER
// ==========================================
class ScoreManager {
public:
    void addPoints(int points) { score_ += points; }
    int getScore() const { return score_; }
    void reset() { score_ = 0; }
    void loadScore(int score) { score_ = score; }

private:
    int score_ = 0;
};
