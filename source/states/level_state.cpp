#include "states/level_state.hpp"
#include "states/play_state.hpp"
#include "services/resource_manager.hpp"
#include "core/game.hpp"
#include <iostream>
#include <cmath>

LevelState::LevelState() {
    const auto& bgTex = ResourceManager::instance().getTexture(Config::OPTION_BG_TEXTURE);
    m_bgOption_ = std::make_unique<sf::Sprite>(bgTex);
    m_bgOption_->setPosition({0.f, 0.f});

    // 2. Cấu hình mặc định cho việc phân trang
    m_currentPage_ = 0;
    m_totalLevels_ = 50;   // Tổng số map bạn có
    m_levelsPerPage_ = 9; // 7 cột x 4 hàng

    // 3. Gọi hàm xây dựng lưới nút bấm lần đầu tiên
    buildLevelButtons();
}

void LevelState::buildLevelButtons() {
    m_buttons_.clear(); // Xóa sạch các nút cũ nếu đang lật trang

    const sf::Font& mainFont = ResourceManager::instance().getFont(Config::MAIN_FONT);
    
    // TẠM DÙNG: Nhớ thêm Config::BTN_BLANK_SQUARE_TEXTURE vào file config.hpp nhé
    const sf::Texture& squareTex = ResourceManager::instance().getTexture(Config::BTN_BLANK_SQUARE_TEXTURE);

    int startLevel = m_currentPage_ * m_levelsPerPage_ + 1;
    int endLevel = std::min(startLevel + m_levelsPerPage_ - 1, m_totalLevels_);

    // --- CẤU HÌNH LƯỚI GRID ---
    float startX = 250.f; // Tọa độ X bắt đầu của vùng 825x518
    float startY = 200.f; // Tọa độ Y bắt đầu của vùng 825x518
    float gapX = 270.f;   // Kích thước nút 71 + Khoảng cách 35
    float gapY = 150.f;
    int cols = 3;         // Số cột

    // 1. KHỞI TẠO CÁC NÚT LEVEL
    for (int i = startLevel; i <= endLevel; ++i) {
        UIButton btn(mainFont);
        btn.sprite = std::make_unique<sf::Sprite>(squareTex);
        centerOrigin(*(btn.sprite));
        
        // MỚI: Phóng to cái bảng gỗ lên 1.3 lần (Tăng 30%)
        btn.sprite->setScale({1.3f, 1.3f}); 
        
        int indexOnPage = i - startLevel; 
        int row = indexOnPage / cols;
        int col = indexOnPage % cols;

        btn.sprite->setPosition({startX + col * gapX, startY + row * gapY});

        btn.text.setString(std::to_string(i));
        
        // MỚI: Tăng size chữ từ 24 lên 36
        btn.text.setCharacterSize(36); 
        btn.text.setFillColor(sf::Color::White);
        
        sf::FloatRect textBounds = btn.text.getLocalBounds();
        btn.text.setOrigin({
            textBounds.position.x + textBounds.size.x / 2.f,
            textBounds.position.y + textBounds.size.y / 2.f
        });
        btn.text.setPosition({startX + col * gapX, startY + row * gapY});
        
        // MỚI: Phóng to luôn cái chữ lên 1.3 lần cho khớp với bảng gỗ
        btn.text.setScale({1.3f, 1.3f}); 

        btn.action = ButtonAction::SelectLevel;
        btn.actionValue = i;

        m_buttons_.push_back(std::move(btn));
    }

    // ==========================================
    // 2. KHỞI TẠO NÚT CHUYỂN TRANG (Phong cách Tối giản & Đồng bộ)
    // ==========================================
    
    // Đặt 2 nút này ở hàng dưới cùng (startY của lưới + thêm khoảng cách)
    float bottomY = startY + 3 * gapY - 30.f; 

    // --- NÚT PREV ("<") ---
    if (m_currentPage_ > 0) {
        UIButton prevBtn(mainFont);
        
        // SỬ DỤNG LẠI ẢNH VUÔNG!
        prevBtn.sprite = std::make_unique<sf::Sprite>(squareTex); 
        centerOrigin(*(prevBtn.sprite));
        prevBtn.sprite->setScale({1.3f, 1.3f}); // Phóng to bằng đúng nút Level
        
        // Đặt thẳng hàng với CỘT ĐẦU TIÊN
        prevBtn.sprite->setPosition({startX, bottomY}); 
        
        // Dùng dấu "<" thay vì chữ PREV
        prevBtn.text.setString("<");
        prevBtn.text.setCharacterSize(45); // Cho dấu mũi tên to rõ lên
        prevBtn.text.setFillColor(sf::Color::White);
        
        sf::FloatRect bounds = prevBtn.text.getLocalBounds();
        prevBtn.text.setOrigin({
            bounds.position.x + bounds.size.x / 2.f, 
            bounds.position.y + bounds.size.y / 2.f
        });
        prevBtn.text.setPosition({startX, bottomY});
        prevBtn.text.setScale({1.3f, 1.3f});
        
        prevBtn.action = ButtonAction::PrevPage;
        m_buttons_.push_back(std::move(prevBtn));
    }

    // ==========================================
    // THÊM MỚI: NÚT BACK Ở CHÍNH GIỮA
    // ==========================================
    UIButton backBtn(mainFont);
    const sf::Texture& rectTex = ResourceManager::instance().getTexture(Config::BTN_BLANK_TEXTURE);
    backBtn.sprite = std::make_unique<sf::Sprite>(rectTex);
    centerOrigin(*(backBtn.sprite));
    
    // Đặt ở CỘT GIỮA (startX + 1 * gapX)
    backBtn.sprite->setPosition({startX + gapX, bottomY});
    
    backBtn.text.setString("BACK");
    backBtn.text.setCharacterSize(30);
    backBtn.text.setFillColor(sf::Color::White);
    
    sf::FloatRect backBounds = backBtn.text.getLocalBounds();
    backBtn.text.setOrigin({
        backBounds.position.x + backBounds.size.x / 2.f, 
        backBounds.position.y + backBounds.size.y / 2.f
    });
    backBtn.text.setPosition({startX + gapX, bottomY});
    
    // Gắn thẻ hành động là Back
    backBtn.action = ButtonAction::Back;
    m_buttons_.push_back(std::move(backBtn));

    // --- NÚT NEXT (">") ---
    if (endLevel < m_totalLevels_) {
        UIButton nextBtn(mainFont);
        
        nextBtn.sprite = std::make_unique<sf::Sprite>(squareTex);
        centerOrigin(*(nextBtn.sprite));
        nextBtn.sprite->setScale({1.3f, 1.3f});
        
        // Đặt thẳng hàng với CỘT CUỐI CÙNG (cột 3)
        nextBtn.sprite->setPosition({startX + 2 * gapX, bottomY}); 
        
        // Dùng dấu ">" thay vì chữ NEXT
        nextBtn.text.setString(">");
        nextBtn.text.setCharacterSize(45);
        nextBtn.text.setFillColor(sf::Color::White);
        
        sf::FloatRect bounds = nextBtn.text.getLocalBounds();
        nextBtn.text.setOrigin({
            bounds.position.x + bounds.size.x / 2.f, 
            bounds.position.y + bounds.size.y / 2.f
        });
        nextBtn.text.setPosition({startX + 2 * gapX, bottomY});
        nextBtn.text.setScale({1.3f, 1.3f});
        
        nextBtn.action = ButtonAction::NextPage;
        m_buttons_.push_back(std::move(nextBtn));
    }
}

void LevelState::processEvents(Game* game, const std::optional<sf::Event>& event) {
    if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPress->code == sf::Keyboard::Key::Escape) {
            game->popState();
            return;
        }
    }
    
    // Xử lý Click chuột (SFML 3.x)
    if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            
            // mapPixelToCoords dùng thẳng mouseEvent->position (Code sạch hơn)
            sf::Vector2f worldPos = game->getWindow().mapPixelToCoords(mouseEvent->position);

            for (const auto& btn : m_buttons_) {
                if (isClicked(*(btn.sprite), worldPos)) {
                    
                    switch (btn.action) {
                        case ButtonAction::SelectLevel:
                            std::cout << "[LOG] Chon Level: " << btn.actionValue << "\n";
                            // TẠM DÙNG: Đổi sang PlayingState, truyền GameMode và Level ID vào
                            game->changeState(new PlayingState(GameMode::CLASSIC, btn.actionValue));
                            break;
                            
                        case ButtonAction::NextPage:
                            m_currentPage_++;
                            buildLevelButtons(); // Render lại trang mới
                            break;
                            
                        case ButtonAction::PrevPage:
                            m_currentPage_--;
                            buildLevelButtons(); // Render lại trang cũ
                            break;
                        
                        case ButtonAction::Back:
                            std::cout << "[LOG] Quay lai man hinh truoc!\n";
                            game->popState(); 
                            break;
                            
                        default:
                            break;
                    }
                    break; // Bấm trúng 1 nút là thoát vòng lặp kiểm tra
                }
            }
        }
    }
}

void LevelState::update(Game* game, float dt) {
    static float time = 0.f;
    time += dt;

    // Định nghĩa mốc Scale chuẩn cho màn hình này
    const float BASE_SCALE = 1.3f; 

    for (auto& btn : m_buttons_) {
        // Chỉ áp dụng hiệu ứng phóng to cho các nút Level (vuông)
        // Bỏ qua nút NEXT/PREV để chúng không bị to quá khổ
        if (btn.action == ButtonAction::SelectLevel) {
            
            if (isMouseHovering(*(btn.sprite), game->getWindow())) {
                
                // MỚI: Nhịp tim lấy mốc từ BASE_SCALE thay vì 1.0f
                float pulse = (BASE_SCALE + 0.05f) + std::sin(time * 15.f) * 0.05f; 
                float jitter = std::cos(time * 30.f) * 1.5f;         
                
                btn.sprite->setScale({pulse, pulse});
                btn.sprite->setRotation(sf::degrees(jitter));
                btn.text.setScale({pulse, pulse});
                btn.text.setRotation(sf::degrees(jitter));
                
                btn.sprite->setColor(sf::Color(255, 150, 150));
                btn.text.setFillColor(sf::Color(255, 20, 20));
            } else {
                
                // MỚI: Nhả chuột ra thì trả về BASE_SCALE thay vì 1.0f
                btn.sprite->setScale({BASE_SCALE, BASE_SCALE});
                btn.sprite->setRotation(sf::degrees(0.f));
                btn.sprite->setColor(sf::Color::White);

                btn.text.setScale({BASE_SCALE, BASE_SCALE});
                btn.text.setRotation(sf::degrees(0.f));
                btn.text.setFillColor(sf::Color::White);
            }
        }
        else {
            if (isMouseHovering(*(btn.sprite), game->getWindow())) {
                float pulse = 1.05f + std::sin(time * 15.f) * 0.05f; 
                float jitter = std::cos(time * 30.f) * 1.5f;         
                
                btn.sprite->setScale({pulse, pulse});
                btn.sprite->setRotation(sf::degrees(jitter));
                btn.text.setScale({pulse, pulse});
                btn.text.setRotation(sf::degrees(jitter));
                
                btn.sprite->setColor(sf::Color(255, 150, 150));
                btn.text.setFillColor(sf::Color(255, 20, 20));
            } else {
                btn.sprite->setScale({1.0f, 1.0f});
                btn.sprite->setRotation(sf::degrees(0.f));
                btn.sprite->setColor(sf::Color::White);

                btn.text.setScale({1.0f, 1.0f});
                btn.text.setRotation(sf::degrees(0.f));
                btn.text.setFillColor(sf::Color::White);
            }
        }
    }
}

void LevelState::render(sf::RenderWindow& window) {
    // Vẽ nút "New Game", "Load Game" ra màn hình
    window.draw(*m_bgOption_);
    for (const auto& btn : m_buttons_) {
        window.draw(*(btn.sprite));
        window.draw(btn.text);
    }
}