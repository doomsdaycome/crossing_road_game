#pragma once

#include <memory>

#include "gameplay/intro_cutscene.hpp"
#include "states/state.hpp"

class ModeState : public State {
 private:
  std::unique_ptr<sf::Sprite> m_bgMenu_;
  std::vector<UIButton> m_buttons_;

  IntroCutscene m_transition_{true};
  int m_selectedLevel_ = -1;

 public:
  ModeState();

  void processEvents(Game* game,
                     const std::optional<sf::Event>& event) override;
  void update(Game* game, float dt) override;
  void render(sf::RenderWindow& window) override;
};