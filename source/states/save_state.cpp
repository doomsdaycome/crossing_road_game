#include "states/save_state.hpp"
#include "services/resource_manager.hpp"
#include "core/game.hpp"

SaveState::SaveState(const GameSnapshot& currentSnapshot) 
    : m_currentSnapshot_(currentSnapshot) {
    
    auto& rm = ResourceManager::instance();
    const sf::Font& mainFont = rm.getFont(Config::MAIN_FONT);
    
    // 1. Background
    m_bgOption_ = std::make_unique<sf::Sprite>(rm.getTexture(Config::OPTION_BG_TEXTURE));
    
    float centerX = Config::WINDOW_WIDTH / 2.f;
    sf::Color darkBrown(60, 40, 20);

    // 2. Tiêu đề FILE SAVE
    m_titleText_ = std::make_unique<sf::Text>(mainFont);
    m_titleText_->setString("FILE SAVE");
    m_titleText_->setCharacterSize(50);
    m_titleText_->setFillColor(darkBrown);
    centerOrigin(*m_titleText_);
    m_titleText_->setPosition({centerX, 150.f});
    
    auto getSlotPath = [](int index) {
        std::string path = Config::DEFAULT_SAVE_PATH;
        size_t dotPos = path.find_last_of('.');
        if (dotPos != std::string::npos) {
            path.insert(dotPos, "_" + std::to_string(index));
        } else {
            path += "_" + std::to_string(index) + ".json"; // Đề phòng file chưa có đuôi
        }
        return path;
    };

    // 3. Khởi tạo 3 Slot Lưu game
    const sf::Texture& rectTex = rm.getTexture(Config::BTN_BLANK_TEXTURE);
    float startY = 300.f;
    float gapY = 120.f; 

    for (int i = 1; i <= 3; ++i) {
        // TRUYỀN FONT VÀO ĐÂY ĐỂ TRÁNH LỖI CONSTRUCTOR
        SaveSlotUI slot(rectTex, mainFont); 
        slot.slotIndex = i;
        
        slot.bg.setTexture(rectTex);
        centerOrigin(slot.bg);
        slot.bg.setScale({1.5f, 1.2f}); 
        slot.bg.setPosition({centerX, startY + (i - 1) * gapY});

        slot.title.setString("SLOT " + std::to_string(i));
        slot.title.setCharacterSize(25);
        slot.title.setFillColor(darkBrown);
        centerOrigin(slot.title);
        slot.title.setPosition({centerX, startY + (i - 1) * gapY - 15.f});

        std::string filepath = getSlotPath(i);
        std::optional<GameSnapshot> savedData = SaveGameRepository::load(filepath);

        slot.details.setCharacterSize(20);
        slot.details.setFillColor(sf::Color(100, 80, 60)); 

        if (savedData.has_value()) {
            slot.isEmpty = false;
            std::string info = "Score: " + std::to_string(savedData->score);
            slot.details.setString(info);
        } else {
            slot.isEmpty = true;
            slot.details.setString("--- EMPTY ---");
        }
        
        centerOrigin(slot.details);
        slot.details.setPosition({centerX, startY + (i - 1) * gapY + 20.f});

        m_slots_.push_back(slot); // Không bị gạch đỏ nữa
    }

    // 4. Khởi tạo Nút BACK và đẩy vào mảng
    UIButton backBtn(mainFont);
    backBtn.sprite = std::make_unique<sf::Sprite>(rectTex);
    centerOrigin(*(backBtn.sprite));
    backBtn.sprite->setPosition({centerX, 700.f});
    
    backBtn.text.setString("BACK");
    backBtn.text.setCharacterSize(30);
    backBtn.text.setFillColor(darkBrown);
    centerOrigin(backBtn.text);
    backBtn.text.setPosition({centerX, 700.f});
    backBtn.action = ButtonAction::Back;
    
    m_buttons_.push_back(std::move(backBtn));
}

void SaveState::processEvents(Game* game, const std::optional<sf::Event>& event) {
    if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPress->code == sf::Keyboard::Key::Escape) {
            game->popState();
            return;
        }
    }
    
    if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            if (!game->isFocused()) return; 

            sf::Vector2f worldPos = game->getWindow().mapPixelToCoords(mouseEvent->position);

            // 1. Check click vào các Slot Save
            for (auto& slot : m_slots_) {
                if (isClicked(slot.bg, worldPos)) {
                    
                    // Tự động chèn biến index để lấy đúng tên file từ Config
                    std::string path = Config::DEFAULT_SAVE_PATH;
                    size_t dotPos = path.find_last_of('.');
                    if (dotPos != std::string::npos) {
                        path.insert(dotPos, "_" + std::to_string(slot.slotIndex));
                    } else {
                        path += "_" + std::to_string(slot.slotIndex) + ".json";
                    }
                    
                    // Gọi hàm lưu với đường dẫn chuẩn
                    bool success = SaveGameRepository::save(path, m_currentSnapshot_);
                    
                    if (success) {
                        slot.isEmpty = false;
                        slot.details.setString("Score: " + std::to_string(m_currentSnapshot_.score));
                        centerOrigin(slot.details);
                        slot.details.setFillColor(sf::Color(20, 150, 20)); 
                    }
                    return; 
                }
            }

            // 2. Check click vào các nút UI (Nút BACK)
            for (const auto& btn : m_buttons_) {
                if (isClicked(*(btn.sprite), worldPos)) {
                    if (btn.action == ButtonAction::Back) {
                        game->popState();
                    }
                    return; 
                }
            }
        }
    }
}

void SaveState::update(Game* game, float dt) {
    static float time = 0.f;
    time += dt;
    
    const float BASE_SCALE_SLOT = 1.5f; // Do lúc nãy mình scale bg của slot là 1.5
    const float BASE_SCALE_BTN = 1.0f;

    // Hover cho các ô Slot
    for (auto& slot : m_slots_) {
        if (isMouseHovering(slot.bg, game->getWindow())) {
            float pulse = BASE_SCALE_SLOT + std::sin(time * 15.f) * 0.02f; 
            slot.bg.setScale({pulse, pulse * 0.8f}); // Nhịp đập nhẹ
            slot.bg.setColor(sf::Color(255, 220, 220)); // Sáng lên xíu
        } else {
            slot.bg.setScale({BASE_SCALE_SLOT, 1.2f});
            slot.bg.setColor(sf::Color::White);
        }
    }

    // Hover cho nút UI (BACK)
    for (auto& btn : m_buttons_) {
        if (isMouseHovering(*(btn.sprite), game->getWindow())) {
            float pulse = BASE_SCALE_BTN + std::sin(time * 15.f) * 0.05f; 
            btn.sprite->setScale({pulse, pulse});
            btn.text.setScale({pulse, pulse});
            btn.sprite->setColor(sf::Color(255, 150, 150));
        } else {
            btn.sprite->setScale({BASE_SCALE_BTN, BASE_SCALE_BTN});
            btn.text.setScale({BASE_SCALE_BTN, BASE_SCALE_BTN});
            btn.sprite->setColor(sf::Color::White);
        }
    }
}

void SaveState::render(sf::RenderWindow& window) {
    window.draw(*m_bgOption_);
    window.draw(*m_titleText_);

    for (const auto& slot : m_slots_) {
        window.draw(slot.bg);
        window.draw(slot.title);
        window.draw(slot.details);
    }

    for (const auto& btn : m_buttons_) {
        window.draw(*(btn.sprite));
        window.draw(btn.text);
    }
}