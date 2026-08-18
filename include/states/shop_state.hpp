#pragma once

#include "states/state.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "gameplay/score_manager.hpp"
#include "utils/animator.hpp"

enum class ItemCategory {
    SKIN,
    BUFF
};

enum class ShopPage {
    SKINS,
    BUFFS
};

struct ShopItem {
    int id;
    std::string name;
    int price;
    ItemCategory category;
    bool isUnlocked;
    int quantity;
    
    std::unique_ptr<sf::Sprite> iconSprite;
    sf::FloatRect cardRect;
    
    Animator animator;
    float directionTimer = 0.f;
    int currentDirection = 0;
};

class ShopState : public State {
public:
    ShopState();
    ~ShopState() override = default;

    void processEvents(Game* game, const std::optional<sf::Event>& event) override;
    void update(Game* game, float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    void initUI();
    void initDummyItems();
    void updateCoinText(int coins);

    ShopPage m_currentPage_ = ShopPage::SKINS;

    std::unique_ptr<sf::Sprite> m_bgSprite_;
    
    std::vector<std::unique_ptr<UIButton>> m_buttons_;
    
    std::unique_ptr<sf::Text> m_titleText_;
    std::unique_ptr<sf::Text> m_coinText_;
    
    std::vector<ShopItem> m_skins_;
    std::vector<ShopItem> m_buffs_;
    
    void renderItemCard(sf::RenderWindow& window, const ShopItem& item, const sf::Font& font);
    
    ScoreManager m_scoreManager_;
};
