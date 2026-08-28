#include "gameplay/intro_cutscene.hpp"
#include "core/config.hpp"
#include "services/resource_manager.hpp"
#include <cstdlib>
#include <cmath>

IntroCutscene::IntroCutscene(bool startIdle) {
    if (startIdle) {
        m_status_ = Status::IDLE;
        m_doorProgress_ = 0.0f; // Cửa mở sẵn
        m_loadingProgress_ = 1.0f;
    } else {
        m_status_ = Status::LOADING;
        m_doorProgress_ = 1.0f; // Cửa đóng kín
        m_loadingProgress_ = 0.0f;
    }
}

void IntroCutscene::startClosing() {
    if (m_status_ == Status::IDLE) {
        m_status_ = Status::CLOSING;
    }
}

bool IntroCutscene::isFinishedOpening() const { return m_status_ == Status::IDLE; }
bool IntroCutscene::isFinishedClosing() const { return m_status_ == Status::CLOSED; }
bool IntroCutscene::isBusy() const { return m_status_ != Status::IDLE && m_status_ != Status::CLOSED; }

void IntroCutscene::update(float dt) {
    if (m_status_ == Status::LOADING) {
        float baseSpeed = 0.25f; 
        float randomStutter = static_cast<float>(std::rand() % 10) * 0.02f; 
        m_loadingProgress_ += dt * (baseSpeed + randomStutter); 
        
        if (m_loadingProgress_ >= 1.0f) {
            m_loadingProgress_ = 1.0f;
            m_status_ = Status::OPENING;
        }
    }
    else if (m_status_ == Status::OPENING) {
        m_doorProgress_ -= dt * 1.5f; 
        if (m_doorProgress_ <= 0.0f) {
            m_doorProgress_ = 0.0f;
            m_status_ = Status::IDLE; // Mở xong thì về trạng thái Nằm im
        }
    }
    else if (m_status_ == Status::CLOSING) {
        m_doorProgress_ += dt * 1.5f; 
        if (m_doorProgress_ >= 1.0f) {
            m_doorProgress_ = 1.0f;
            m_status_ = Status::CLOSED; // Đóng xong
        }
    }
}

void IntroCutscene::render(sf::RenderWindow& window) {
    if (m_status_ == Status::IDLE) return; // Đang nằm im (cửa mở toang) thì không vẽ gì cả

    float halfWidth = Config::WINDOW_WIDTH / 2.f;
    float height = Config::WINDOW_HEIGHT;
    float offset = halfWidth * (1.0f - m_doorProgress_);

    const sf::Texture& texLeft = ResourceManager::instance().getTexture(Config::DOOR_LEFT_TEXTURE);
    const sf::Texture& texRight = ResourceManager::instance().getTexture(Config::DOOR_RIGHT_TEXTURE);

    sf::Sprite leftDoor(texLeft);
    sf::Sprite rightDoor(texRight);

    float imgW = static_cast<float>(texLeft.getSize().x);
    float imgH = static_cast<float>(texLeft.getSize().y);
    leftDoor.setScale({halfWidth / imgW, height / imgH});
    rightDoor.setScale({halfWidth / imgW, height / imgH});

    leftDoor.setPosition({-offset, 0.f});
    rightDoor.setPosition({halfWidth + offset, 0.f});

    window.draw(leftDoor);
    window.draw(rightDoor);

    // CHỈ VẼ LOADING BAR NẾU ĐANG Ở TRẠNG THÁI LOADING
    if (m_status_ == Status::LOADING) {
        float barWidth = 600.f;
        float barHeight = 4.f; 
        float barX = (Config::WINDOW_WIDTH - barWidth) / 2.f;
        float barY = Config::WINDOW_HEIGHT - 80.f; 

        sf::RectangleShape border({barWidth + 4.f, barHeight + 4.f});
        border.setPosition({barX - 2.f, barY - 2.f});
        border.setFillColor(sf::Color::Transparent);
        border.setOutlineThickness(1.f);
        border.setOutlineColor(sf::Color(100, 100, 100, 120));

        sf::RectangleShape track({barWidth, barHeight});
        track.setPosition({barX, barY});
        track.setFillColor(sf::Color(20, 20, 20, 220)); 

        sf::RectangleShape glow({barWidth * m_loadingProgress_, barHeight + 10.f});
        glow.setPosition({barX, barY - 5.f});
        glow.setFillColor(sf::Color(180, 10, 10, 60));

        sf::RectangleShape fill({barWidth * m_loadingProgress_, barHeight});
        fill.setPosition({barX, barY});
        fill.setFillColor(sf::Color(220, 20, 20, 255)); 

        sf::RectangleShape head({4.f, barHeight + 2.f});
        head.setPosition({barX + (barWidth * m_loadingProgress_) - 4.f, barY - 1.f});
        head.setFillColor(sf::Color::White); 
        if (m_loadingProgress_ <= 0.01f) head.setFillColor(sf::Color::Transparent); 

        const sf::Font& mainFont = ResourceManager::instance().getFont(Config::MAIN_FONT);
        sf::Text loadText(mainFont);
        int percent = static_cast<int>(m_loadingProgress_ * 100);
        loadText.setString("L O A D I N G . . .   " + std::to_string(percent) + "%");
        loadText.setCharacterSize(16);
        
        static sf::Clock glowClock;
        float time = glowClock.getElapsedTime().asSeconds();
        std::uint8_t alpha = 160 + static_cast<std::uint8_t>(std::abs(std::sin(time * 6.f)) * 95);
        loadText.setFillColor(sf::Color(255, 200, 200, alpha));

        sf::FloatRect textBounds = loadText.getLocalBounds();
        loadText.setOrigin({
            textBounds.position.x + textBounds.size.x / 2.f, 
            textBounds.position.y + textBounds.size.y / 2.f
        });
        loadText.setPosition({Config::WINDOW_WIDTH / 2.f, barY - 25.f});

        window.draw(border);
        window.draw(track);
        window.draw(glow);
        window.draw(fill);
        window.draw(head);
        window.draw(loadText);
    }
}