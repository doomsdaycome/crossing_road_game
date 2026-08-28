#pragma once
#include "states/state.hpp"
#include "services/save_game_repository.hpp" 
#include <vector>
#include <memory>
#include <string>

// Cấu trúc đại diện cho 1 ô lưu game
struct SaveSlotUI {
    sf::Sprite bg;
    sf::Text title;   
    sf::Text details; 
    int slotIndex;    
    bool isEmpty;

    // BẮT BUỘC CÓ CONSTRUCTOR: Để truyền font cho 2 cái Text ngay từ đầu
    SaveSlotUI(const sf::Texture& texture, const sf::Font& font) 
        : bg(texture), title(font), details(font) {}
};

class SaveState : public State {
private:
    std::unique_ptr<sf::Sprite> m_bgOption_;
    std::unique_ptr<sf::Text> m_titleText_;
    
    std::vector<SaveSlotUI> m_slots_;
    std::vector<UIButton> m_buttons_; // Dùng mảng vector thay vì một biến đơn lẻ
    
    GameSnapshot m_currentSnapshot_; 

public:
    SaveState(const GameSnapshot& currentSnapshot); 
    void processEvents(Game* game, const std::optional<sf::Event>& event) override;
    void update(Game* game, float dt) override;
    void render(sf::RenderWindow& window) override;
};