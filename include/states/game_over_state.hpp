#pragma once
#include "states/state.hpp"
#include "states/play_state.hpp"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class GameOverState : public State {
private:
    std::unique_ptr<sf::Text> m_titleText_;
    std::unique_ptr<sf::Text> m_scoreText_;
    
    // --- THÊM 2 BIẾN NÀY VÀO ĐỂ FILE CPP KHÔNG BÁO LỖI ---
    std::unique_ptr<sf::Text> m_goldText_;       
    std::unique_ptr<sf::Text> m_highScoreText_;  
    
    std::vector<UIButton> m_buttons_;
    
    // Lớp màng mờ phủ lên trên Gameplay
    sf::RectangleShape m_bgOverlay_; 

    int m_finalScore_;
    GameMode m_mode_;
    int m_level_;

    // --- CÁC BIẾN NHẬN DỮ LIỆU TỪ PLAYING STATE ---
    bool m_isVictory_;   
    int m_earnedGold_;   
    int m_oldGold_;      
    int m_highScore_;    

    // --- BIẾN DÙNG ĐỂ CHẠY HIỆU ỨNG ĐẾM SỐ ---
    float m_displayScore_; 
    float m_displayGold_;

    // --- CÁC BIẾN CHO ANIMATION TRƯỢT ---
    float m_leftX_;       // Tọa độ X hiện tại của nhóm Trái
    float m_rightX_;      // Tọa độ X hiện tại của nhóm Phải
    float m_targetX_;     // Tọa độ đích (Chính giữa màn hình)

    bool m_hasCollided_;  // Cờ báo hiệu 2 bên đã tông vào nhau
    float m_shakeTimer_;  // Thời gian rung màn hình sau cú tông

public:
    // SỬA Ở ĐÂY: Thêm = false và = 0 để làm giá trị mặc định
    GameOverState(int score, GameMode mode, int level, bool isVictory = false, int earnedGold = 0, int bestScore = 0, int totalCoins = 0);
    
    void processEvents(Game* game, const std::optional<sf::Event>& event) override;
    void update(Game* game, float dt) override;
    void render(sf::RenderWindow& window) override;
};