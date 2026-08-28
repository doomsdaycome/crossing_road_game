#include <cstdlib>
#include <ctime>

#include "core/game.hpp"

int main() {
  srand(static_cast<unsigned int>(time(nullptr)));

  Game game;
  game.run();
  return 0;
}