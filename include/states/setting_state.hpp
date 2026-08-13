#pragma once
#include "states/state.hpp"
#include "services/save_game_repository.hpp"
#include <memory>

class SettingState : public State {
private:
    std::unique_ptr<sf::Sprite> m_bgOption_;
    std::vector<UIButton> m_buttons_;

    std::unique_ptr<sf::Text> m_musicLabel_;
    std::unique_ptr<sf::Text> m_musicValueText_;
    std::unique_ptr<sf::Text> m_sfxLabel_;
    std::unique_ptr<sf::Text> m_sfxValueText_;
    std::unique_ptr<sf::Text> m_themeLabel_;
    std::unique_ptr<sf::Text> m_themeValueText_;

    bool m_isFromGameplay_;
    GameSnapshot m_currentSnapshot_;

    int m_themeIndex_ = 0;
    std::vector<std::string> m_themeOptions_ = {"CLASSIC", "PHARAOH", "DARK TOMB"};
public:
    SettingState(bool isFromGameplay = false, GameSnapshot snapshot = GameSnapshot());
    
    void processEvents(Game* game, const std::optional<sf::Event>& event) override;
    void update(Game* game, float dt) override;
    void render(sf::RenderWindow& window) override;
};