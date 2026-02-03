#include "game.h"
#include <memory>

int main() {
    auto game = std::make_unique<BabaIsYou::Game>();
    game->Loop();

    return 0;
}
