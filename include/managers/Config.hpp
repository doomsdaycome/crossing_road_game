#pragma once

#include <SFML/System.hpp>

enum class GameLevel {
  UNKNOW_LEVEL = 0,
  EASY = 1,
  NORMAL = 2,
  HARD = 3,
};

enum class GameState {
  UNKNOW_STATE = 0,
  LOAD = 1,
  MENU = 2,
  PLAY = 3,
};

enum class WindowMode {
  UNKNOWN_MODE = 0,
  WINDOWED = 1,
  BORDERLESS = 2,
  FULLSCREEN = 3,
};

class WindowConfig {
private:
  inline static const sf::Vector2u DEFAULT_SIZE{1280, 720};

  sf::Vector2u size_ = DEFAULT_SIZE;
  float scale_ = 1.0f;
  float ratio_ = 16.0f / 9.0f;
  int frame_rate_ = 60;
  WindowMode mode_ = WindowMode::WINDOWED;

public:
  WindowConfig() = default;

  WindowConfig(const WindowConfig &other) = default;
  WindowConfig(WindowConfig &&other) noexcept = default;

  ~WindowConfig() = default;

  WindowConfig &operator=(const WindowConfig &other) = default;
  WindowConfig &operator=(WindowConfig &&other) noexcept = default;

  sf::Vector2u getSize() const;
  int getWidth() const;
  int getHeight() const;
  float getScale() const;
  float getRatio() const;
  int getFrameRate() const;
  WindowMode getMode() const;

  void setSize(const sf::Vector2u &size);
  void setWidth(int width);
  void setHeight(int height);
  void setScale(float scale);
  void setRatio(float ratio);
  void setFrameRate(int frame_rate);
  void setMode(WindowMode mode);
};

class GameConfig {
private:
  GameState game_state_ = GameState::LOAD;
  GameLevel game_level_ = GameLevel::EASY;
  WindowConfig window_config_ = WindowConfig();

public:
  WindowConfig &getWindowConfig();
  const WindowConfig &getWindowConfig() const;
};
