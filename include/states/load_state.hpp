#pragma once
#include "states/state.hpp"
#include "services/save_game_repository.hpp" 
#include <vector>
#include <memory>
#include <string>

struct LoadSlotUI {
    sf::Sprite bg;
    sf::Text title;   
    sf::Text details; 
    int slotIndex;    
    bool isEmpty;
    
    // Biến này sẽ ôm sẵn gói dữ liệu nếu file tồn tại
    std::optional<GameSnapshot> snapshotData; 

    LoadSlotUI(const sf::Texture& texture, const sf::Font& font) 
        : bg(texture), title(font), details(font) {}
};

class LoadState : public State {
private:
    std::unique_ptr<sf::Sprite> m_bgOption_;
    std::unique_ptr<sf::Text> m_titleText_;
    
    std::vector<LoadSlotUI> m_slots_;
    std::vector<UIButton> m_buttons_; 

public:
    LoadState(); 
    void processEvents(Game* game, const std::optional<sf::Event>& event) override;
    void update(Game* game, float dt) override;
    void render(sf::RenderWindow& window) override;
};