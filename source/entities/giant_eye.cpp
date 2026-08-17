#include "entities/giant_eye.hpp" // Thay đường dẫn cho đúng thư mục của ông
#include "core/config.hpp"
#include "services/resource_manager.hpp"
#include "utils/utils.hpp"
#include <cstdlib>
#include <cmath>
#include <cstdint>

GiantEye::GiantEye() 
    : m_state(State::HIDDEN),
      // MỚI: Cấp Texture ngay lúc khai sinh để SFML 3.x không báo lỗi
      m_sprite(ResourceManager::instance().getTexture(Config::EYE_FRAME_0)),
      // Khởi tạo luôn size cho nền đỏ (SFML 3.x constructor của RectangleShape)
      m_redOverlay(sf::Vector2f{Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}),
      m_currentFrame(0),
      m_alpha(0.f),
      m_scale(1.0f)
{
    m_state = State::HIDDEN;
    m_alpha = 0.f;
    m_scale = 1.0f;
    m_currentFrame = 0;
    
    auto& rm = ResourceManager::instance();
    m_textures.push_back(&rm.getTexture(Config::EYE_FRAME_0));
    m_textures.push_back(&rm.getTexture(Config::EYE_FRAME_1));
    m_textures.push_back(&rm.getTexture(Config::EYE_FRAME_2));
    m_textures.push_back(&rm.getTexture(Config::EYE_FRAME_3));
    m_textures.push_back(&rm.getTexture(Config::EYE_FRAME_4));
    m_textures.push_back(&rm.getTexture(Config::EYE_FRAME_5));

    m_sprite.setTexture(*m_textures[0]);
    centerOrigin(m_sprite);
    
    // Đặt ở vị trí giữa màn hình, nhích lên trên một chút (tọa độ Y = 180)
    m_sprite.setPosition(sf::Vector2f{Config::WINDOW_WIDTH / 2.f, 180.f}); 

    // Setup nền đỏ mờ toàn màn hình
    m_redOverlay.setSize(sf::Vector2f{Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT});
    m_redOverlay.setFillColor(sf::Color(0, 0, 0, 0)); 

    resetSpawnCooldown();
}

void GiantEye::resetSpawnCooldown() {
    // Random xuất hiện từ 15 đến 30 giây một lần (Hiếm)
    m_timer = 15.f + static_cast<float>(std::rand() % 16);
}

void GiantEye::update(float dt) {
    switch (m_state) {
        case State::HIDDEN:
            m_timer -= dt;
            if (m_timer <= 0.f) {
                m_state = State::FADE_IN;
                m_currentFrame = 0;
                m_sprite.setTexture(*m_textures[0]);
                m_alpha = 0.f;
            }
            break;

        case State::FADE_IN:
            m_alpha += 200.f * dt; 
            if (m_alpha >= 255.f) {
                m_alpha = 255.f;
                m_state = State::OPENING;
                m_timer = 0.4f; // Thời gian chờ để chuyển frame
            }
            m_sprite.setColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(m_alpha)));
            break;

        case State::OPENING:
            m_timer -= dt;
            if (m_timer <= 0.f) {
                m_currentFrame++;
                if (m_currentFrame >= 4) {
                    // Đã tới Frame 4 -> Chuyển sang Gồng (Charging)
                    m_state = State::CHARGING;
                    m_timer = 0.35f; // Gồng trong 0.35s
                    
                    // Thụt về Frame 3 để tạo cảm giác lấy đà
                    m_sprite.setTexture(*m_textures[3]);
                    
                    // [Tùy chọn] Bật SFX gồng: rm.playSound(Config::SFX_EYE_CHARGE);
                } else {
                    m_sprite.setTexture(*m_textures[m_currentFrame]);
                    m_timer = 0.4f; 
                }
            }
            break;

        case State::CHARGING:
            m_timer -= dt;
            // Hiệu ứng nén nhỏ lại (Từ 1.0 rớt xuống 0.7)
            m_scale = 2.0f - (0.35f - m_timer) * 2.f; 
            if (m_scale < 1.5f) m_scale = 1.5f;
            m_sprite.setScale(sf::Vector2f{m_scale, m_scale});

            if (m_timer <= 0.f) {
                // BÙNG NỔ!
                m_state = State::ACTIVE_RED_LIGHT;
                m_sprite.setTexture(*m_textures[5]);
                m_scale = 3.2f; 
                m_sprite.setScale(sf::Vector2f{m_scale, m_scale});
                m_timer = 3.f + static_cast<float>(std::rand() % 3);
                
                // [Tùy chọn] Bật SFX cảnh báo: rm.playSound(Config::SFX_RED_LIGHT);
            }
            break;

        case State::ACTIVE_RED_LIGHT:
            m_timer -= dt;
            
            // Ép màu đỏ mờ lên màn hình
            m_redOverlay.setFillColor(sf::Color(200, 0, 0, 70)); 
            
            // Hiệu ứng nhịp đập (pulse) cho con mắt thêm phần ghê rợn
            m_scale = 3.0f + 0.2f * std::sin(m_timer * 15.f);
            m_sprite.setScale(sf::Vector2f{m_scale, m_scale});

            if (m_timer <= 0.f) {
                m_state = State::FADE_OUT;
                m_sprite.setTexture(*m_textures[0]); // Nhắm mắt lại
                m_scale = 2.0f;
                m_sprite.setScale(sf::Vector2f{m_scale, m_scale});
                m_redOverlay.setFillColor(sf::Color(0, 0, 0, 0)); // Tắt nền đỏ
            }
            break;

        case State::FADE_OUT:
            m_alpha -= 250.f * dt;
            if (m_alpha <= 0.f) {
                m_alpha = 0.f;
                m_state = State::HIDDEN;
                resetSpawnCooldown(); // Reset chờ đợt xuất hiện tiếp theo
            }
            m_sprite.setColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(m_alpha)));
            break;
    }
}

void GiantEye::render(sf::RenderWindow& window) {
    if (m_state == State::ACTIVE_RED_LIGHT) {
        window.draw(m_redOverlay);
    }
    if (m_state != State::HIDDEN) {
        window.draw(m_sprite);
    }
}

bool GiantEye::isRedLightActive() const {
    return m_state == State::ACTIVE_RED_LIGHT;
}