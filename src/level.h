#pragma once

#include "tile.h"
#include <array>

namespace BabaIsYou {

constexpr int LEVEL_WIDTH = 33;
constexpr int LEVEL_HEIGHT = 18;
constexpr int NUM_LEVEL = 2;

struct Vec2i {
    int x;
    int y;
};

struct GameState {
    std::array<std::array<Tile, LEVEL_WIDTH>, LEVEL_HEIGHT> tiles;
    bool isWin;
};

/*
class Level {
    std::array<std::array<char, LEVEL_WIDTH + 1>, LEVEL_HEIGHT> m_tiles;
};

class LevelManager {
  public:
    GameState LoadLevel(int index) { GameState gs;

    }

    void NextLevel() {
        if (m_currentLevel < NUM_LEVEL) {
            m_currentLevel++;
        }
    }
    void PreviousLevel() {
        if (m_currentLevel > 0) {
            m_currentLevel--;
        }
    }

  private:
    int m_currentLevel = 0;
    std::array<Level, NUM_LEVEL> m_levels;
};*/

const std::array<std::array<char, LEVEL_WIDTH + 1>, LEVEL_HEIGHT>& GetLevel(int index);

} // namespace BabaIsYou
