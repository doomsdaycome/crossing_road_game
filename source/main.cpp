#include "core/game.hpp"
#include <ctime>
#include <cstdlib>

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    Game game;
    game.run();
    return 0;
}