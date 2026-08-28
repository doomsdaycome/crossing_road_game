#pragma once

#include <memory>

#include "gameplay/intro_cutscene.hpp"
#include "states/state.hpp"

class LevelState : public State {
 private:
  std::unique_ptr<sf::Sprite> m_bgOption_;
  std::vector<UIButton> m_buttons_;

  int m_currentPage_;
  int m_totalLevels_;
  int m_levelsPerPage_;

  IntroCutscene m_transition_{true};
  int m_selectedLevel_ = -1;

  std::vector<sf::Text> m_coinTexts_;

  void buildLevelButtons();

 public:
  LevelState();

  void processEvents(Game* game,
                     const std::optional<sf::Event>& event) override;
  void update(Game* game, float dt) override;
  void render(sf::RenderWindow& window) override;
};