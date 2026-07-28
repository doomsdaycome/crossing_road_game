#pragma once

#include "states/state.hpp"

// MOI: state hoan toan thieu o ban goc - khong co cach nao ket thuc van choi truoc day.
class GameOverState : public State {
public:
    explicit GameOverState(int finalScore);

    void processEvents(Game* game, const std::optional<sf::Event>& event) override;
    void update(Game* game, float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    int finalScore_;
};
