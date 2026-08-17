#include "states/menu_state.hpp"
#include "states/play_state.hpp"
#include "states/save_state.hpp"
#include "states/setting_state.hpp"
#include "services/resource_manager.hpp"
#include "core/game.hpp"
#include <iostream>

SettingState::SettingState(bool isFromGameplay, GameSnapshot snapshot) 
    : m_isFromGameplay_(isFromGameplay), m_currentSnapshot_(snapshot) {
    const auto& bgTex = ResourceManager::instance().getTexture(Config::OPTION_BG_TEXTURE);
    m_bgOption_ = std::make_unique<sf::Sprite>(bgTex);
    m_bgOption_->setPosition({0.f, 0.f});

    // Lấy Font và Texture nút
    const sf::Font& mainFont = ResourceManager::instance().getFont(Config::MAIN_FONT);
    const sf::Texture& squareTex = ResourceManager::instance().getTexture(Config::BTN_BLANK_SQUARE_TEXTURE);
    const sf::Texture& rectTex = ResourceManager::instance().getTexture(Config::BTN_BLANK_TEXTURE);
    sf::Color darkBrown(60, 40, 20);

    float centerX = Config::WINDOW_WIDTH / 2.f - 50.f;
    float startY = 180.f;
    float gapY = 150.f;

    // ==========================================
    // HÀNG 1: ĐIỀU CHỈNH MUSIC
    // ==========================================
    // Nhãn "MUSIC"
    m_musicLabel_ = std::make_unique<sf::Text>(mainFont);
    m_musicLabel_->setString("MUSIC");
    m_musicLabel_->setCharacterSize(40);
    m_musicLabel_->setPosition({centerX - 250.f, startY});

    // Nút Giảm Music (<)
    UIButton musicDownBtn(mainFont);
    musicDownBtn.sprite = std::make_unique<sf::Sprite>(squareTex);
    centerOrigin(*(musicDownBtn.sprite));
    musicDownBtn.sprite->setPosition({centerX + 50.f, startY + 20.f});
    musicDownBtn.text.setString("<");
    musicDownBtn.text.setCharacterSize(35);
    centerOrigin(musicDownBtn.text);
    musicDownBtn.text.setPosition({centerX + 50.f, startY + 20.f});
    musicDownBtn.action = ButtonAction::MusicDown;
    m_buttons_.push_back(std::move(musicDownBtn));

    // Hiển thị số Music (%)
    m_musicValueText_ = std::make_unique<sf::Text>(mainFont);
    int currentMusic = static_cast<int>(ResourceManager::instance().getMusicVolume());
    m_musicValueText_->setString(std::to_string(currentMusic) + "%");
    m_musicValueText_->setCharacterSize(35);
    centerOrigin(*m_musicValueText_);
    m_musicValueText_->setPosition({centerX + 150.f, startY + 20.f});

    // Nút Tăng Music (>)
    UIButton musicUpBtn(mainFont);
    musicUpBtn.sprite = std::make_unique<sf::Sprite>(squareTex);
    centerOrigin(*(musicUpBtn.sprite));
    musicUpBtn.sprite->setPosition({centerX + 250.f, startY + 20.f});
    musicUpBtn.text.setString(">");
    musicUpBtn.text.setCharacterSize(35);
    centerOrigin(musicUpBtn.text);
    musicUpBtn.text.setPosition({centerX + 250.f, startY + 20.f});
    musicUpBtn.action = ButtonAction::MusicUp;
    m_buttons_.push_back(std::move(musicUpBtn));

    // Nút Tắt/Bật nhanh Music (Mute)
    UIButton musicMuteBtn(mainFont);
    musicMuteBtn.sprite = std::make_unique<sf::Sprite>(squareTex);
    centerOrigin(*(musicMuteBtn.sprite));
    musicMuteBtn.sprite->setPosition({centerX + 350.f, startY + 20.f}); // Nằm bên phải nút >
    
    float initMusicVol = ResourceManager::instance().getMusicVolume();
    musicMuteBtn.text.setString(initMusicVol > 0.f ? "M" : "X");
    musicMuteBtn.text.setCharacterSize(35);
    musicMuteBtn.text.setFillColor(darkBrown);
    centerOrigin(musicMuteBtn.text);
    musicMuteBtn.text.setPosition({centerX + 350.f, startY + 20.f});
    
    musicMuteBtn.action = ButtonAction::MusicMute;
    m_buttons_.push_back(std::move(musicMuteBtn));


    // ==========================================
    // HÀNG 2: ĐIỀU CHỈNH SFX
    // ==========================================
    // (Lặp lại logic tương tự cho SFX, nhích startY xuống 1 khoảng gapY)
    m_sfxLabel_ = std::make_unique<sf::Text>(mainFont);
    m_sfxLabel_->setString("SFX");
    m_sfxLabel_->setCharacterSize(40);
    m_sfxLabel_->setPosition({centerX - 250.f, startY + gapY});

    UIButton sfxDownBtn(mainFont);
    sfxDownBtn.sprite = std::make_unique<sf::Sprite>(squareTex);
    centerOrigin(*(sfxDownBtn.sprite));
    sfxDownBtn.sprite->setPosition({centerX + 50.f, startY + gapY + 20.f});
    sfxDownBtn.text.setString("<");
    sfxDownBtn.text.setCharacterSize(35);
    centerOrigin(sfxDownBtn.text);
    sfxDownBtn.text.setPosition({centerX + 50.f, startY + gapY + 20.f});
    sfxDownBtn.action = ButtonAction::SfxDown;
    m_buttons_.push_back(std::move(sfxDownBtn));

    m_sfxValueText_ = std::make_unique<sf::Text>(mainFont);
    int currentSfx = static_cast<int>(ResourceManager::instance().getSfxVolume());
    m_sfxValueText_->setString(std::to_string(currentSfx) + "%");
    m_sfxValueText_->setCharacterSize(35);
    centerOrigin(*(m_sfxValueText_));
    m_sfxValueText_->setPosition({centerX + 150.f, startY + gapY + 20.f});

    UIButton sfxUpBtn(mainFont);
    sfxUpBtn.sprite = std::make_unique<sf::Sprite>(squareTex);
    centerOrigin(*(sfxUpBtn.sprite));
    sfxUpBtn.sprite->setPosition({centerX + 250.f, startY + gapY + 20.f});
    sfxUpBtn.text.setString(">");
    sfxUpBtn.text.setCharacterSize(35);
    centerOrigin(sfxUpBtn.text);
    sfxUpBtn.text.setPosition({centerX + 250.f, startY + gapY + 20.f});
    sfxUpBtn.action = ButtonAction::SfxUp;
    m_buttons_.push_back(std::move(sfxUpBtn));

    m_musicLabel_->setFillColor(darkBrown);
    m_musicValueText_->setFillColor(darkBrown);
    m_sfxLabel_->setFillColor(darkBrown);
    m_sfxValueText_->setFillColor(darkBrown);

    musicDownBtn.text.setFillColor(darkBrown);
    musicUpBtn.text.setFillColor(darkBrown);
    sfxDownBtn.text.setFillColor(darkBrown);
    sfxUpBtn.text.setFillColor(darkBrown);

    // Nút Tắt/Bật nhanh SFX (Mute)
    UIButton sfxMuteBtn(mainFont);
    sfxMuteBtn.sprite = std::make_unique<sf::Sprite>(squareTex);
    centerOrigin(*(sfxMuteBtn.sprite));
    sfxMuteBtn.sprite->setPosition({centerX + 350.f, startY + gapY + 20.f});
    
    float initSfxVol = ResourceManager::instance().getSfxVolume();
    sfxMuteBtn.text.setString(initSfxVol > 0.f ? "M" : "X");
    sfxMuteBtn.text.setCharacterSize(35);
    sfxMuteBtn.text.setFillColor(darkBrown);
    centerOrigin(sfxMuteBtn.text);
    sfxMuteBtn.text.setPosition({centerX + 350.f, startY + gapY + 20.f});
    
    sfxMuteBtn.action = ButtonAction::SfxMute;
    m_buttons_.push_back(std::move(sfxMuteBtn));

    // ==========================================
    // HÀNG 3: CHỌN THEME (GIAO DIỆN)
    // ==========================================
    m_themeLabel_ = std::make_unique<sf::Text>(mainFont);
    m_themeLabel_->setString("THEME");
    m_themeLabel_->setCharacterSize(40);
    m_themeLabel_->setFillColor(darkBrown);
    m_themeLabel_->setPosition({centerX - 250.f, startY + 2.f * gapY});

    // Nút <
    UIButton themeDownBtn(mainFont);
    themeDownBtn.sprite = std::make_unique<sf::Sprite>(squareTex);
    centerOrigin(*(themeDownBtn.sprite));
    themeDownBtn.sprite->setPosition({centerX + 50.f, startY + 2.f * gapY + 20.f});
    themeDownBtn.text.setString("<");
    themeDownBtn.text.setCharacterSize(35);
    themeDownBtn.text.setFillColor(darkBrown);
    centerOrigin(themeDownBtn.text);
    themeDownBtn.text.setPosition({centerX + 50.f, startY + 2.f * gapY + 20.f});
    themeDownBtn.action = ButtonAction::ThemePrev; // Nhớ khai báo ThemePrev trong enum nhé
    m_buttons_.push_back(std::move(themeDownBtn));

    // Hiển thị tên Theme hiện tại
    m_themeValueText_ = std::make_unique<sf::Text>(mainFont);
    m_themeValueText_->setString(m_themeOptions_[m_themeIndex_]);
    m_themeValueText_->setCharacterSize(35);
    m_themeValueText_->setFillColor(darkBrown);
    centerOrigin(*m_themeValueText_);
    m_themeValueText_->setPosition({centerX + 200.f, startY + 2.f * gapY + 20.f});

    // Nút >
    UIButton themeUpBtn(mainFont);
    themeUpBtn.sprite = std::make_unique<sf::Sprite>(squareTex);
    centerOrigin(*(themeUpBtn.sprite));
    themeUpBtn.sprite->setPosition({centerX + 350.f, startY + 2.f * gapY + 20.f});
    themeUpBtn.text.setString(">");
    themeUpBtn.text.setCharacterSize(35);
    themeUpBtn.text.setFillColor(darkBrown);
    centerOrigin(themeUpBtn.text);
    themeUpBtn.text.setPosition({centerX + 350.f, startY + 2.f * gapY + 20.f});
    themeUpBtn.action = ButtonAction::ThemeNext; // Khai báo ThemeNext trong enum
    m_buttons_.push_back(std::move(themeUpBtn));


    // ==========================================
    // HÀNG CUỐI: CÁC NÚT ĐIỀU HƯỚNG BÊN DƯỚI
    // ==========================================
    float bottomY = startY + 3.0f * gapY; 

    // --- 1. NÚT BACK (Luôn hiển thị) ---
    UIButton backBtn(mainFont);
    backBtn.sprite = std::make_unique<sf::Sprite>(rectTex);
    centerOrigin(*(backBtn.sprite));
    
    backBtn.text.setString("BACK");
    backBtn.text.setCharacterSize(30);
    backBtn.text.setFillColor(darkBrown);
    centerOrigin(backBtn.text);
    
    backBtn.action = ButtonAction::Back;
    
    // --- 2. NÚT HOME (Luôn hiển thị) ---
    UIButton homeBtn(mainFont);
    homeBtn.sprite = std::make_unique<sf::Sprite>(rectTex);
    centerOrigin(*(homeBtn.sprite));

    homeBtn.text.setString("HOME");
    homeBtn.text.setCharacterSize(30);
    homeBtn.text.setFillColor(darkBrown);
    centerOrigin(homeBtn.text);
    
    homeBtn.action = ButtonAction::Home;

    // --- 3. DÀN TRẬN TỌA ĐỘ VÀ NÚT SAVE ---
    if (m_isFromGameplay_) {
        // NẾU TỪ GAME: Vẽ 3 nút dàn đều ra
        backBtn.sprite->setPosition({centerX - 200.f, bottomY});
        backBtn.text.setPosition({centerX - 200.f, bottomY});
        
        homeBtn.sprite->setPosition({centerX + 300.f, bottomY});
        homeBtn.text.setPosition({centerX + 300.f, bottomY});
        
        // Tạo nút SAVE đặt chình ình ở giữa
        UIButton saveBtn(mainFont);
        saveBtn.sprite = std::make_unique<sf::Sprite>(rectTex);
        centerOrigin(*(saveBtn.sprite));
        saveBtn.sprite->setPosition({centerX + 50.f, bottomY}); // Đặt ngay giữa
        
        saveBtn.text.setString("SAVE");
        saveBtn.text.setCharacterSize(30);
        saveBtn.text.setFillColor(darkBrown);
        centerOrigin(saveBtn.text);
        saveBtn.text.setPosition({centerX + 50.f, bottomY});
        
        saveBtn.action = ButtonAction::SaveGame; // KHAI BÁO ACTION NÀY VÀO ENUM NHÉ
        m_buttons_.push_back(std::move(saveBtn));
    } else {
        // NẾU TỪ MENU: Vẽ 2 nút cân xứng 2 bên như cũ
        backBtn.sprite->setPosition({centerX - 100.f, bottomY});
        backBtn.text.setPosition({centerX - 100.f, bottomY});
        
        homeBtn.sprite->setPosition({centerX + 200.f, bottomY});
        homeBtn.text.setPosition({centerX + 200.f, bottomY});
    }

    // Đẩy BACK và HOME vào mảng
    m_buttons_.push_back(std::move(backBtn));
    m_buttons_.push_back(std::move(homeBtn));
}

void SettingState::processEvents(Game* game, const std::optional<sf::Event>& event) {
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
            auto& rm = ResourceManager::instance();

            for (auto& btn : m_buttons_) {
                if (isClicked(*(btn.sprite), worldPos)) {
                    
                    float currentVol;
                    
                    switch (btn.action) {
                        case ButtonAction::MusicDown:
                            currentVol = rm.getMusicVolume() - 10.f;
                            rm.setMusicVolume(currentVol);
                            m_musicValueText_->setString(std::to_string((int)rm.getMusicVolume()) + "%");
                            centerOrigin(*m_musicValueText_);
                            
                            // ĐỒNG BỘ NÚT MUTE: Tìm nút Mute và đổi chữ nếu về 0
                            for (auto& b : m_buttons_) {
                                if (b.action == ButtonAction::MusicMute) {
                                    b.text.setString(rm.getMusicVolume() > 0.f ? "M" : "X");
                                    centerOrigin(b.text);
                                    break;
                                }
                            }
                            break;
                            
                        case ButtonAction::MusicUp:
                            currentVol = rm.getMusicVolume() + 10.f;
                            rm.setMusicVolume(currentVol);
                            m_musicValueText_->setString(std::to_string((int)rm.getMusicVolume()) + "%");
                            centerOrigin(*m_musicValueText_);

                            // ĐỒNG BỘ NÚT MUTE: Tìm nút Mute và đổi chữ nếu > 0
                            for (auto& b : m_buttons_) {
                                if (b.action == ButtonAction::MusicMute) {
                                    b.text.setString(rm.getMusicVolume() > 0.f ? "M" : "X");
                                    centerOrigin(b.text);
                                    break;
                                }
                            }
                            break;

                        case ButtonAction::MusicMute:
                            currentVol = rm.getMusicVolume();
                            if (currentVol > 0.f) {
                                m_lastMusicVol_ = currentVol;
                                rm.setMusicVolume(0.f);
                                btn.text.setString("X");
                            } else {
                                // NẾU BẬT LẠI MÀ ÂM LƯỢNG CŨ LÀ 0, THÌ GÁN LÊN 10%
                                rm.setMusicVolume(m_lastMusicVol_ > 0.f ? m_lastMusicVol_ : 1.f);
                                btn.text.setString("M");
                            }
                            centerOrigin(btn.text);
                            
                            m_musicValueText_->setString(std::to_string((int)rm.getMusicVolume()) + "%");
                            centerOrigin(*m_musicValueText_);
                            break;


                        // ==========================================
                        // XỬ LÝ SFX (Y chang Music)
                        // ==========================================
                        case ButtonAction::SfxDown:
                            currentVol = rm.getSfxVolume() - 10.f;
                            rm.setSfxVolume(currentVol);
                            m_sfxValueText_->setString(std::to_string((int)rm.getSfxVolume()) + "%");
                            centerOrigin(*m_sfxValueText_);
                            
                            for (auto& b : m_buttons_) {
                                if (b.action == ButtonAction::SfxMute) {
                                    b.text.setString(rm.getSfxVolume() > 0.f ? "M" : "X");
                                    centerOrigin(b.text);
                                    break;
                                }
                            }
                            break;

                        case ButtonAction::SfxUp:
                            currentVol = rm.getSfxVolume() + 10.f;
                            rm.setSfxVolume(currentVol);
                            m_sfxValueText_->setString(std::to_string((int)rm.getSfxVolume()) + "%");
                            centerOrigin(*m_sfxValueText_);
                            
                            for (auto& b : m_buttons_) {
                                if (b.action == ButtonAction::SfxMute) {
                                    b.text.setString(rm.getSfxVolume() > 0.f ? "M" : "X");
                                    centerOrigin(b.text);
                                    break;
                                }
                            }
                            break;

                        case ButtonAction::SfxMute:
                            currentVol = rm.getSfxVolume();
                            if (currentVol > 0.f) {
                                m_lastSfxVol_ = currentVol;
                                rm.setSfxVolume(0.f);
                                btn.text.setString("X");
                            } else {
                                // GÁN LÊN 10% NẾU CŨ BỊ ÉP VỀ 0
                                rm.setSfxVolume(m_lastSfxVol_ > 0.f ? m_lastSfxVol_ : 1.f);
                                btn.text.setString("M");
                            }
                            centerOrigin(btn.text);
                            
                            m_sfxValueText_->setString(std::to_string((int)rm.getSfxVolume()) + "%");
                            centerOrigin(*m_sfxValueText_);
                            break;

                        case ButtonAction::ThemePrev:
                            m_themeIndex_--; // Lùi lại 1 index
                            
                            // Nếu lùi quá số 0 thì vòng ngược lại theme cuối cùng
                            if (m_themeIndex_ < 0) {
                                m_themeIndex_ = m_themeOptions_.size() - 1; 
                            }
                            
                            // Cập nhật chữ hiển thị và căn giữa lại
                            m_themeValueText_->setString(m_themeOptions_[m_themeIndex_]);
                            centerOrigin(*m_themeValueText_);
                            
                            // [MỞ RỘNG TƯƠNG LAI] Ông có thể gọi hàm báo cho Shop 
                            // hoặc ResourceManager đổi giao diện ngay tại đây
                            break;

                        case ButtonAction::ThemeNext:
                            m_themeIndex_++; // Tiến tới 1 index
                            
                            // Nếu tiến lố qua tổng số theme thì vòng lại theme đầu tiên (số 0)
                            if (m_themeIndex_ >= m_themeOptions_.size()) {
                                m_themeIndex_ = 0; 
                            }
                            
                            // Cập nhật chữ hiển thị và căn giữa lại
                            m_themeValueText_->setString(m_themeOptions_[m_themeIndex_]);
                            centerOrigin(*m_themeValueText_);
                            break;

                        case ButtonAction::Back:
                            game->popState();
                            break;

                        case ButtonAction::SaveGame:
                            // TODO: Gọi logic save ở đây
                            game->pushState(new SaveState(m_currentSnapshot_));
                            break;

                        case ButtonAction::Home:
                            game->changeState(new MenuState());
                            break;
                    }
                    break; 
                }
            }
        }
    }
}

void SettingState::update(Game* game, float dt) {
    static float time = 0.f;
    time += dt;
    
    const float BASE_SCALE = 1.0f; 

    for (auto& btn : m_buttons_) {
        if (isMouseHovering(*(btn.sprite), game->getWindow())) {
            float pulse = BASE_SCALE + std::sin(time * 15.f) * 0.05f; 
            btn.sprite->setScale({pulse, pulse});
            btn.text.setScale({pulse, pulse});
            btn.sprite->setColor(sf::Color(255, 150, 150));
            btn.text.setFillColor(sf::Color(255, 20, 20));
        } else {
            btn.sprite->setScale({BASE_SCALE, BASE_SCALE});
            btn.sprite->setColor(sf::Color::White);
            btn.text.setScale({BASE_SCALE, BASE_SCALE});
            btn.text.setFillColor(sf::Color::White);
        }
    }
}

void SettingState::render(sf::RenderWindow& window) {
    window.draw(*m_bgOption_);

    window.draw(*m_musicLabel_);
    window.draw(*m_musicValueText_);
    window.draw(*m_sfxLabel_);
    window.draw(*m_sfxValueText_);
    window.draw(*m_themeValueText_);
    window.draw(*m_themeLabel_);

    // Vẽ toàn bộ nút
    for (const auto& btn : m_buttons_) {
        window.draw(*(btn.sprite));
        window.draw(btn.text);
    }
}