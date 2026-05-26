#include "managers/Config.hpp"

#include <SFML/System.hpp>

sf::Vector2u WindowConfig::getSize() const { return size_; }
int WindowConfig::getWidth() const { return static_cast<int>(size_.x); }
int WindowConfig::getHeight() const { return static_cast<int>(size_.y); }
float WindowConfig::getScale() const { return scale_; }
float WindowConfig::getRatio() const { return ratio_; }
int WindowConfig::getFrameRate() const { return frame_rate_; }
WindowMode WindowConfig::getMode() const { return mode_; }

void WindowConfig::setSize(const sf::Vector2u &size) {
  size_ = size;

  if (size_.y != 0)
    ratio_ = static_cast<float>(size_.x) / size_.y;
}

void WindowConfig::setWidth(int width) {
  if (width < 0)
    return;

  size_.x = static_cast<unsigned int>(width);
  size_.y = static_cast<unsigned int>(size_.x / ratio_);
}

void WindowConfig::setHeight(int height) {
  if (height < 0)
    return;

  size_.y = static_cast<unsigned int>(height);
  size_.x = static_cast<unsigned int>(ratio_ * size_.y);
}

void WindowConfig::setScale(float scale) {
  scale_ = scale;
  size_.x = static_cast<unsigned int>(DEFAULT_SIZE.x * scale_);
  size_.y = static_cast<unsigned int>(DEFAULT_SIZE.y * scale_);
}

void WindowConfig::setRatio(float ratio) {
  if (ratio <= 0.0f)
    return;

  ratio_ = ratio;
  size_.y = static_cast<unsigned int>(size_.x / ratio_);
}

void WindowConfig::setFrameRate(int frame_rate) { frame_rate_ = frame_rate; }

void WindowConfig::setMode(WindowMode mode) {
  if (mode == WindowMode::UNKNOWN_MODE)
    return;

  mode_ = mode;
}

WindowConfig &GameConfig::getWindowConfig() { return window_config_; }

const WindowConfig &GameConfig::getWindowConfig() const {
  return window_config_;
}
