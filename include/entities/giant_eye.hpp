#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class GiantEye {
 public:
  enum class State {
    HIDDEN,            // Đang trốn, đếm ngược chờ xuất hiện
    FADE_IN,           // Mờ dần hiện ra (Frame 0)
    OPENING,           // Mở từ từ (Frame 0 -> 4)
    CHARGING,          // Gồng! (Thụt về Frame 3, nén lại)
    ACTIVE_RED_LIGHT,  // Bùng nổ (Frame 5) - ĐÈN ĐỎ
    FADE_OUT           // Trở lại Frame 0 và mờ dần biến mất
  };

  GiantEye();
  void update(float dt);

  // Gọi hàm này khi đang ở View mặc định (UI) để mắt luôn nằm cố định trên màn
  // hình
  void render(sf::RenderWindow& window);

  bool isRedLightActive() const;

 private:
  State m_state;
  sf::Sprite m_sprite;
  std::vector<const sf::Texture*> m_textures;
  sf::RectangleShape m_redOverlay;  // Lớp kính đỏ mờ che màn hình

  float m_timer;  // Đồng hồ đếm ngược cho các State
  int m_currentFrame;
  float m_alpha;  // Độ mờ (0 - 255)
  float m_scale;  // Tỉ lệ phóng to/thu nhỏ

  void resetSpawnCooldown();
};