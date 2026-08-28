#include "states/shop_state.hpp"

#include <iostream>

#include "core/config.hpp"
#include "core/game.hpp"
#include "services/resource_manager.hpp"
#include "utils/utils.hpp"

ShopState::ShopState() {
  m_scoreManager_.loadHighScore();
  initUI();
  initDummyItems();
}

void ShopState::initUI() {
  auto& resMgr = ResourceManager::instance();
  const sf::Font& font = resMgr.getFont(Config::MAIN_FONT);

  // Load Background
  const sf::Texture& bgTex = resMgr.getTexture(Config::SHOP_BG_TEXTURE);
  m_bgSprite_ = std::make_unique<sf::Sprite>(bgTex);

  // Title
  m_titleText_ = std::make_unique<sf::Text>(font);
  m_titleText_->setString("SHOP");
  m_titleText_->setCharacterSize(50);
  m_titleText_->setFillColor(sf::Color::Yellow);
  m_titleText_->setOutlineColor(sf::Color::Black);
  m_titleText_->setOutlineThickness(3.f);
  centerOrigin(*m_titleText_);
  m_titleText_->setPosition({660.f, 150.f});

  // Coin text
  m_coinText_ = std::make_unique<sf::Text>(font);
  m_coinText_->setCharacterSize(30);
  m_coinText_->setFillColor(sf::Color::White);
  m_coinText_->setOutlineColor(sf::Color::Black);
  m_coinText_->setOutlineThickness(2.f);
  m_coinText_->setPosition({220.f, 150.f});

  // Navigation Buttons
  const sf::Texture& btnBlankTex = resMgr.getTexture(Config::BTN_BLANK_TEXTURE);

  // Back Button
  auto backBtn = std::make_unique<UIButton>(font);
  backBtn->sprite = std::make_unique<sf::Sprite>(btnBlankTex);
  backBtn->action = ButtonAction::Back;
  backBtn->text.setString("Back");
  backBtn->text.setCharacterSize(30);
  centerOrigin(*backBtn->sprite);
  backBtn->sprite->setPosition(
      {Config::WINDOW_WIDTH / 2.f, Config::WINDOW_HEIGHT - 80.f});
  centerOrigin(backBtn->text);
  backBtn->text.setPosition(backBtn->sprite->getPosition());
  m_buttons_.push_back(std::move(backBtn));

  // Prev Page Button
  auto prevBtn = std::make_unique<UIButton>(font);
  prevBtn->sprite = std::make_unique<sf::Sprite>(btnBlankTex);
  prevBtn->action = ButtonAction::PrevPage;
  prevBtn->text.setString("Skins");
  prevBtn->text.setCharacterSize(25);
  centerOrigin(*prevBtn->sprite);
  prevBtn->sprite->setScale({0.7f, 0.8f});
  prevBtn->sprite->setPosition({260.f, 300.f});
  centerOrigin(prevBtn->text);
  prevBtn->text.setPosition(prevBtn->sprite->getPosition());
  m_buttons_.push_back(std::move(prevBtn));

  // Next Page Button
  auto nextBtn = std::make_unique<UIButton>(font);
  nextBtn->sprite = std::make_unique<sf::Sprite>(btnBlankTex);
  nextBtn->action = ButtonAction::NextPage;
  nextBtn->text.setString("Buffs");
  nextBtn->text.setCharacterSize(25);
  centerOrigin(*nextBtn->sprite);
  nextBtn->sprite->setScale({0.7f, 0.8f});
  nextBtn->sprite->setPosition({260.f, 400.f});
  centerOrigin(nextBtn->text);
  nextBtn->text.setPosition(nextBtn->sprite->getPosition());
  m_buttons_.push_back(std::move(nextBtn));
}

void ShopState::initDummyItems() {
  // In a real game, item stats should be loaded from a file or config.
  // SKINS
  for (int i = 0; i < 4; ++i) {
    ShopItem item;
    item.id = i;
    item.name = "Skin " + std::to_string(i + 1);
    item.price = 100 * (i + 1);
    item.category = ItemCategory::SKIN;
    item.isUnlocked =
        m_scoreManager_.isSkinUnlocked(i);  // Read from ScoreManager
    item.quantity = 1;

    // Define Rect Layout
    float x = 450.f + (i % 2) * 250.f;
    float y = 250.f + (i / 2) * 200.f;
    item.cardRect = sf::FloatRect({x, y}, {200.f, 150.f});

    // --- LOAD SKIN ANIMATION ---
    std::string texPath =
        "asset/images/entities/player/pl" + std::to_string(i) + ".png";
    const sf::Texture& tex = ResourceManager::instance().getTextureWithMask(
        texPath, sf::Color::Magenta);
    item.iconSprite = std::make_unique<sf::Sprite>(tex);

    int frameWidth = tex.getSize().x / 4;
    int frameHeight = tex.getSize().y / 4;
    // Animator(frameWidth, frameHeight, frameCount, frameDuration)
    // Chỉnh thời gian mỗi frame từ 0.15f lên 0.35f để nhân vật đi tản bộ chậm
    // rãi hơn
    item.animator = Animator(frameWidth, frameHeight, 4, 0.35f);
    item.iconSprite->setTextureRect(item.animator.getTextureRect());

    // Đặt ở góc phải của thẻ bài
    item.iconSprite->setOrigin({frameWidth / 2.f, frameHeight / 2.f});
    item.iconSprite->setPosition({x + 140.f, y + 60.f});
    item.iconSprite->setScale({1.0f, 1.0f});

    m_skins_.push_back(std::move(item));
  }

  // BUFFS
  std::string buffNames[4] = {"Ring of Greed", "Blood Aegis", "Phantom Cloak",
                              "Holy Nova"};
  int buffCounts[4] = {
      m_scoreManager_.getMagnetCount(), m_scoreManager_.getShieldCount(),
      m_scoreManager_.getInvisCount(), m_scoreManager_.getNovaCount()};

  for (int i = 0; i < 4; ++i) {
    ShopItem item;
    item.id = 100 + i;
    item.name = buffNames[i];
    item.price = 50 * (i + 1);
    item.category = ItemCategory::BUFF;
    item.isUnlocked = true;
    item.quantity = buffCounts[i];

    // Define Rect Layout
    float x = 450.f + (i % 2) * 250.f;
    float y = 250.f + (i / 2) * 200.f;
    item.cardRect = sf::FloatRect({x, y}, {200.f, 150.f});

    std::string buffTexPaths[4] = {
        Config::BUFF_RING_TEXTURE, Config::BUFF_SHIELD_TEXTURE,
        Config::BUFF_CLOAK_TEXTURE, Config::BUFF_NOVA_TEXTURE};

    const sf::Texture& tex =
        ResourceManager::instance().getTexture(buffTexPaths[i]);
    item.iconSprite = std::make_unique<sf::Sprite>(tex);

    // Scale 500x500 xuống cỡ 60x60
    float scale = 60.f / tex.getSize().x;
    item.iconSprite->setScale({scale, scale});
    item.iconSprite->setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
    item.iconSprite->setPosition({x + 150.f, y + 60.f});

    m_buffs_.push_back(std::move(item));
  }
}

void ShopState::updateCoinText(int coins) {
  m_coinText_->setString("Coins: " + std::to_string(coins));
}

void ShopState::processEvents(Game* game,
                              const std::optional<sf::Event>& event) {
  if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
    if (mousePress->button == sf::Mouse::Button::Left) {
      sf::Vector2f mousePos(static_cast<float>(mousePress->position.x),
                            static_cast<float>(mousePress->position.y));

      // Check Button Clicks
      for (const auto& btn : m_buttons_) {
        if (btn->sprite->getGlobalBounds().contains(mousePos)) {
          if (btn->action == ButtonAction::Back) {
            game->popState();
            return;
          } else if (btn->action == ButtonAction::NextPage) {
            m_currentPage_ = ShopPage::BUFFS;
          } else if (btn->action == ButtonAction::PrevPage) {
            m_currentPage_ = ShopPage::SKINS;
          }
        }
      }

      // Check Item Card Clicks
      std::vector<ShopItem>& currentItems =
          (m_currentPage_ == ShopPage::SKINS) ? m_skins_ : m_buffs_;
      for (auto& item : currentItems) {
        if (item.cardRect.contains(mousePos)) {
          // Try to purchase
          if (!item.isUnlocked || item.category == ItemCategory::BUFF) {
            if (item.category == ItemCategory::BUFF && item.quantity >= 5) {
              std::cout << "Maxed out: " << item.name << "\n";
            } else if (m_scoreManager_.spendCoins(item.price)) {
              // Success!
              if (item.category == ItemCategory::SKIN) {
                item.isUnlocked = true;
                m_scoreManager_.unlockSkin(item.id);
                m_scoreManager_.saveHighScore();
              } else {
                item.quantity++;
                if (item.id == 100)
                  m_scoreManager_.addMagnet();
                else if (item.id == 101)
                  m_scoreManager_.addShield();
                else if (item.id == 102)
                  m_scoreManager_.addInvis();
                else if (item.id == 103)
                  m_scoreManager_.addNova();
                m_scoreManager_.saveHighScore();
              }
              std::cout << "Bought: " << item.name << "\n";
            } else {
              std::cout << "Not enough coins for: " << item.name << "\n";
            }
          } else if (item.isUnlocked && item.category == ItemCategory::SKIN) {
            std::cout << "Equipped: " << item.name << "\n";
            m_scoreManager_.setEquippedSkin(item.id);
            m_scoreManager_.saveHighScore();
          }
        }
      }
    }
  }
}

void ShopState::update(Game* game, float dt) {
  updateCoinText(m_scoreManager_.getTotalCoins());

  // Hover effects
  sf::Vector2i mousePos = sf::Mouse::getPosition(game->getWindow());
  sf::Vector2f mousePosF(static_cast<float>(mousePos.x),
                         static_cast<float>(mousePos.y));

  for (auto& btn : m_buttons_) {
    if (btn->sprite->getGlobalBounds().contains(mousePosF)) {
      btn->sprite->setColor(sf::Color(200, 200, 200));
    } else {
      btn->sprite->setColor(sf::Color::White);
    }
  }

  // --- CẬP NHẬT HOẠT ẢNH CHO SKINS ---
  for (auto& item : m_skins_) {
    if (item.iconSprite) {
      item.animator.update(dt);
      item.directionTimer += dt;

      // Xoay hướng sau mỗi 1.5 giây
      if (item.directionTimer > 1.5f) {
        item.currentDirection = (item.currentDirection + 1) % 4;
        item.animator.setDirection(item.currentDirection);
        item.directionTimer = 0.f;
      }
      item.iconSprite->setTextureRect(item.animator.getTextureRect());
    }
  }
}

void ShopState::renderItemCard(sf::RenderWindow& window, const ShopItem& item,
                               const sf::Font& font) {
  // Draw background card
  sf::RectangleShape cardShape(item.cardRect.size);
  cardShape.setPosition(item.cardRect.position);
  cardShape.setFillColor(sf::Color(0, 0, 0, 150));
  cardShape.setOutlineThickness(2.f);
  cardShape.setOutlineColor(item.isUnlocked &&
                                    item.category == ItemCategory::SKIN
                                ? sf::Color::Green
                                : sf::Color::White);
  window.draw(cardShape);

  // Draw item name
  sf::Text nameText(font);
  nameText.setString(item.name);
  nameText.setCharacterSize(20);
  nameText.setPosition(
      {item.cardRect.position.x + 10.f, item.cardRect.position.y + 10.f});
  window.draw(nameText);

  // Draw price or status
  sf::Text statusText(font);
  statusText.setCharacterSize(18);
  statusText.setPosition(
      {item.cardRect.position.x + 10.f, item.cardRect.position.y + 110.f});

  if (item.category == ItemCategory::SKIN && item.isUnlocked) {
    if (m_scoreManager_.getEquippedSkin() == item.id) {
      statusText.setString("Equipped");
      statusText.setFillColor(sf::Color::Cyan);
    } else {
      statusText.setString("Owned / Equip");
      statusText.setFillColor(sf::Color::Green);
    }
  } else if (item.category == ItemCategory::BUFF && item.quantity >= 5) {
    statusText.setString("MAX (5/5)");
    statusText.setFillColor(sf::Color(150, 150, 150));
  } else {
    if (item.category == ItemCategory::BUFF) {
      statusText.setString("Price: " + std::to_string(item.price) +
                           " (Own: " + std::to_string(item.quantity) + ")");
    } else {
      statusText.setString("Price: " + std::to_string(item.price));
    }
    statusText.setFillColor(sf::Color::Yellow);
  }
  window.draw(statusText);

  // Removed redundant qtyText drawing

  // Draw icon/animation if exists
  if (item.iconSprite) {
    window.draw(*item.iconSprite);
  }
}

void ShopState::render(sf::RenderWindow& window) {
  window.draw(*m_bgSprite_);
  window.draw(*m_titleText_);
  window.draw(*m_coinText_);

  for (const auto& btn : m_buttons_) {
    window.draw(*btn->sprite);
    window.draw(btn->text);
  }

  const sf::Font& font = ResourceManager::instance().getFont(Config::MAIN_FONT);

  const auto& currentItems =
      (m_currentPage_ == ShopPage::SKINS) ? m_skins_ : m_buffs_;
  for (const auto& item : currentItems) {
    renderItemCard(window, item, font);
  }
}
