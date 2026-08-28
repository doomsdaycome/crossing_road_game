#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Template dùng chung cho mọi vật thể SFML
template <typename T>
bool isMouseHovering(const T& object, const sf::RenderWindow& window) {
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos);
    
    return object.getGlobalBounds().contains(mouseWorldPos);
}

template <typename T>
bool isClicked(const T& object, sf::Vector2f mouseWorldPos) {
    // Trong hàm processEvents phải có logic lấy tọa độ chuột trước
    return object.getGlobalBounds().contains(mouseWorldPos);
}

/// <summary>
/// Tự động tính toán và dời điểm neo (Origin) của vật thể về đúng chính giữa tâm.
/// Hỗ trợ hoàn hảo cho sf::Text (tự bù trừ độ lệch chân chữ).
/// </summary>
/// <typeparam name="T">Kiểu vật thể (sf::Text, sf::Sprite, sf::RectangleShape...)</typeparam>
/// <param name="object">Vật thể cần dời tâm</param>
template <typename T>
inline void centerOrigin(T& object) {
    // Phải dùng getLocalBounds() thay vì getGlobalBounds() để lấy kích thước gốc nguyên bản
    sf::FloatRect bounds = object.getLocalBounds();
    
    object.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
    });
}