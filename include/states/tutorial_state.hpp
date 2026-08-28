#pragma once
#include "states/state.hpp"
#include <memory>
#include <string>

class TutorialState : public State {
private:
    std::unique_ptr<sf::Sprite> m_bgMenu_;
    std::unique_ptr<sf::Sprite> m_guide_;
    std::unique_ptr<sf::Sprite> m_scrollTop_;

    sf::Text m_titleText_;
    UIButton m_backButton_;
    UIButton m_topButton_;

    sf::View m_scrollView_;
    float m_scrollOffset_ = 0.f;
    float m_maxScroll_ = 0.f;

    float m_clipY_;
    float m_clipH_;

public:
    TutorialState();
    void processEvents(Game* game, const std::optional<sf::Event>& event) override;
    void update(Game* game, float dt) override;
    void render(sf::RenderWindow& window) override;
};
