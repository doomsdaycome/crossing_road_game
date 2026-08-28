#pragma once

#include <SFML/Graphics.hpp>

#include "core/config.hpp"

// ==========================================
// CAMERA CONTROLLER
// Tach logic camera (sf::View + cuon man hinh) ra khoi PlayingState.
// ==========================================
class CameraController {
 public:
  CameraController() {
    view_.setSize({Config::VIEW_WIDTH, Config::VIEW_HEIGHT});
    view_.setCenter({Config::VIEW_WIDTH / 2.f, Config::VIEW_HEIGHT / 2.f});
  }

  void update(float dt, float speedMultiplier) {
    float scrollSpeed = Config::CAMERA_BASE_SCROLL_SPEED * speedMultiplier;
    view_.move({0.f, -scrollSpeed * dt});
  }

  const sf::View& getView() const { return view_; }

  float getTopEdge() const {
    return view_.getCenter().y - (view_.getSize().y / 2.f);
  }
  float getBottomEdge() const {
    return view_.getCenter().y + (view_.getSize().y / 2.f);
  }
  void setCenterY(float y) { view_.setCenter({view_.getCenter().x, y}); }

 private:
  sf::View view_;
};
