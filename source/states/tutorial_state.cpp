#include "states/tutorial_state.hpp"

#include <cmath>
#include <iostream>

#include "core/config.hpp"
#include "core/game.hpp"
#include "services/resource_manager.hpp"
#include "states/menu_state.hpp"

TutorialState::TutorialState()
    : m_titleText_(ResourceManager::instance().getFont(Config::MAIN_FONT)),
      m_backButton_(ResourceManager::instance().getFont(Config::MAIN_FONT)),
      m_topButton_(ResourceManager::instance().getFont(Config::MAIN_FONT)) {
  auto& rm = ResourceManager::instance();

  m_bgMenu_ =
      std::make_unique<sf::Sprite>(rm.getTexture(Config::MENU_BG_TEXTURE));

  m_backButton_.action = ButtonAction::Back;
  m_backButton_.sprite = std::make_unique<sf::Sprite>(
      rm.getTexture(Config::BTN_BLANK_SQUARE_TEXTURE));
  centerOrigin(*(m_backButton_.sprite));
  m_backButton_.sprite->setPosition({80.f, 80.f});
  m_backButton_.sprite->setScale({0.7f, 0.7f});
  m_backButton_.text.setString("BACK");
  m_backButton_.text.setCharacterSize(20);
  m_backButton_.text.setFillColor(sf::Color::White);
  centerOrigin(m_backButton_.text);
  m_backButton_.text.setPosition({80.f, 80.f});

  m_topButton_.action = ButtonAction::None;
  m_topButton_.sprite = std::make_unique<sf::Sprite>(
      rm.getTexture(Config::BTN_BLANK_SQUARE_TEXTURE));
  centerOrigin(*(m_topButton_.sprite));
  m_topButton_.sprite->setPosition(
      {Config::WINDOW_WIDTH - 100.f, Config::WINDOW_HEIGHT - 80.f});
  m_topButton_.sprite->setScale({0.7f, 0.7f});
  m_topButton_.text.setString("TOP");
  m_topButton_.text.setCharacterSize(20);
  m_topButton_.text.setFillColor(sf::Color::White);
  centerOrigin(m_topButton_.text);
  m_topButton_.text.setPosition(
      {Config::WINDOW_WIDTH - 100.f, Config::WINDOW_HEIGHT - 80.f});

  const auto& guideTex = rm.getTexture(Config::GUIDE_TEXTURE);
  m_guide_ = std::make_unique<sf::Sprite>(guideTex);

  // ==========================================
  // KHU VỰC TỰ CHỈNH VỊ TRÍ
  // ==========================================

  // 1. Vị trí ảnh cuộn giấy (guide.png)
  // Cast to float to prevent unsigned integer underflow!
  float startX = (static_cast<float>(Config::WINDOW_WIDTH) -
                  static_cast<float>(guideTex.getSize().x)) /
                 2.f;
  float startY = -323.f;

  // 2. Vị trí thanh gỗ che (scroll_top.png)
  // Tự do thay đổi số ở đây (bạn đang để 71 và 187)
  float scrollTopX = 71.f;
  float scrollTopY = 15.f;
  float topScale = 1.0f;

  // 3. Vị trí Y của màn che (Ranh giới mà cuộn giấy chui xuống và biến mất)
  // Nếu màn che bị quá cao hoặc thấp, bạn chỉ cần thay đổi số m_clipY_ này!
  m_clipY_ = scrollTopY + 45.f;  // Cộng thêm một đoạn để giấy nằm dưới mép gỗ
  m_clipH_ = static_cast<float>(Config::WINDOW_HEIGHT) - m_clipY_;

  // ==========================================

  m_guide_->setPosition({std::round(startX), std::round(startY)});

  const auto& topTex = rm.getTexture(Config::SCROLL_TOP_TEXTURE);
  m_scrollTop_ = std::make_unique<sf::Sprite>(topTex);
  m_scrollTop_->setScale({topScale, topScale});
  m_scrollTop_->setPosition({std::round(scrollTopX), std::round(scrollTopY)});

  m_titleText_.setString("SURVIVAL GUIDE");
  m_titleText_.setCharacterSize(40);
  m_titleText_.setFillColor(sf::Color::White);
  m_titleText_.setOutlineColor(sf::Color::Black);
  m_titleText_.setOutlineThickness(3.f);
  centerOrigin(m_titleText_);
  m_titleText_.setPosition({Config::WINDOW_WIDTH / 2.f, 80.f});

  m_scrollView_.setViewport(sf::FloatRect(
      {0.f, m_clipY_ / static_cast<float>(Config::WINDOW_HEIGHT)},
      {1.f, m_clipH_ / static_cast<float>(Config::WINDOW_HEIGHT)}));
  m_scrollView_.setSize({static_cast<float>(Config::WINDOW_WIDTH), m_clipH_});
  m_scrollView_.setCenter(
      {Config::WINDOW_WIDTH / 2.f, m_clipY_ + m_clipH_ / 2.f});

  float totalScrollHeight = startY + static_cast<float>(guideTex.getSize().y);
  m_maxScroll_ =
      totalScrollHeight - static_cast<float>(Config::WINDOW_HEIGHT) + 100.f;
  if (m_maxScroll_ < 0.f) m_maxScroll_ = 0.f;
}

void TutorialState::processEvents(Game* game,
                                  const std::optional<sf::Event>& event) {
  if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
    if (mouseEvent->button == sf::Mouse::Button::Left) {
      sf::Vector2f mousePos = game->getWindow().mapPixelToCoords(
          mouseEvent->position, game->getWindow().getDefaultView());

      if (m_backButton_.sprite->getGlobalBounds().contains(mousePos)) {
        game->popState();
      } else if (m_scrollOffset_ > 200.f &&
                 m_topButton_.sprite->getGlobalBounds().contains(mousePos)) {
        m_scrollOffset_ = 0.f;
        m_scrollView_.setCenter(
            {Config::WINDOW_WIDTH / 2.f, m_clipY_ + m_clipH_ / 2.f});
      }
    }
  } else if (const auto* scrollEvent =
                 event->getIf<sf::Event::MouseWheelScrolled>()) {
    if (scrollEvent->wheel == sf::Mouse::Wheel::Vertical) {
      float scrollAmount = scrollEvent->delta * -40.f;
      m_scrollOffset_ += scrollAmount;

      if (m_scrollOffset_ < 0.f) m_scrollOffset_ = 0.f;
      if (m_scrollOffset_ > m_maxScroll_) m_scrollOffset_ = m_maxScroll_;

      m_scrollView_.setCenter({Config::WINDOW_WIDTH / 2.f,
                               m_clipY_ + m_clipH_ / 2.f + m_scrollOffset_});
    }
  }
}

void TutorialState::update(Game* game, float dt) {
  sf::Vector2i pixelPos = sf::Mouse::getPosition(game->getWindow());
  sf::Vector2f mousePos = game->getWindow().mapPixelToCoords(
      pixelPos, game->getWindow().getDefaultView());

  if (m_backButton_.sprite->getGlobalBounds().contains(mousePos)) {
    m_backButton_.sprite->setColor(sf::Color(255, 200, 200));
    m_backButton_.sprite->setScale({0.75f, 0.75f});
    m_backButton_.text.setScale({1.05f, 1.05f});
  } else {
    m_backButton_.sprite->setColor(sf::Color::White);
    m_backButton_.sprite->setScale({0.7f, 0.7f});
    m_backButton_.text.setScale({1.0f, 1.0f});
  }

  if (m_scrollOffset_ > 200.f) {
    if (m_topButton_.sprite->getGlobalBounds().contains(mousePos)) {
      m_topButton_.sprite->setColor(sf::Color(255, 200, 200));
      m_topButton_.sprite->setScale({0.75f, 0.75f});
      m_topButton_.text.setScale({1.05f, 1.05f});
    } else {
      m_topButton_.sprite->setColor(sf::Color::White);
      m_topButton_.sprite->setScale({0.7f, 0.7f});
      m_topButton_.text.setScale({1.0f, 1.0f});
    }
  }
}

void TutorialState::render(sf::RenderWindow& window) {
  window.setView(window.getDefaultView());
  window.draw(*m_bgMenu_);

  // Vẽ phần thân giấy cuộn trong view bị giới hạn (clip)
  window.setView(m_scrollView_);
  window.draw(*m_guide_);

  // Đổi lại view mặc định để vẽ đè phần đầu cuộn cố định
  window.setView(window.getDefaultView());
  window.draw(*m_scrollTop_);

  window.draw(*(m_backButton_.sprite));
  window.draw(m_backButton_.text);

  if (m_scrollOffset_ > 200.f) {
    window.draw(*(m_topButton_.sprite));
    window.draw(m_topButton_.text);
  }
}
