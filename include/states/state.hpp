#pragma once
#include <SFML/Graphics.hpp>
#include "utils/utils.hpp"
#include <optional>

class Game;

// ==========================================
// ĐỊNH NGHĨA HÀNH ĐỘNG NÚT DÙNG CHUNG CHO TOÀN GAME
// ==========================================
enum class ButtonAction {
    None,       // Mặc định, không làm gì cả
    Start,      // Bắt đầu game
    Continue,   // Chơi tiếp
    Shop,       // Vào cửa hàng
    Scoreboard, // Xem bảng xếp hạng
    Option,     // Cài đặt
    Exit,       // Thoát game
    Back,       // (Dành cho OptionState/ShopState) Trở về màn hình trước
    Classic,
    Endless,
    SelectLevel,
    NextPage,
    PrevPage
};

// ==========================================
// CẤU TRÚC NÚT BẤM DÙNG CHUNG CHO MỌI STATE
// ==========================================
struct UIButton {
    std::unique_ptr<sf::Sprite> sprite;
    sf::Text text;
    ButtonAction action = ButtonAction::None;

    int actionValue = -1;

    // Khởi tạo bắt buộc phải có Font (Đặc thù của SFML 3.x)
    explicit UIButton(const sf::Font& font) : text(font) {}
};

class State {
public:
    virtual ~State() = default;

    virtual void processEvents(Game* game, const std::optional<sf::Event>& event) = 0;
    virtual void update(Game* game, float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};