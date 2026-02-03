#pragma once

#include "level.h"
#include "tile.h"
#include <string>
#include <vector>

namespace BabaIsYou {

constexpr float TILE_PIXEL_SIZE = 48.0f;
constexpr int SCREEN_WIDTH = TILE_PIXEL_SIZE * LEVEL_WIDTH;
constexpr int SCREEN_HEIGHT = TILE_PIXEL_SIZE * LEVEL_HEIGHT;
constexpr size_t MAX_HISTORY = 512;

struct Vec2i {
    int x;
    int y;
};

class Game {
  public:
    Game();
    ~Game();

    void Loop();

  private:
    void Update();
    void Draw() const;

    static bool InBounds(int x, int y);
    void LoadLevel(int number);
    void TryMove(int dx, int dy);

    void SaveState();
    void Undo();

    struct GameState {
        std::array<std::array<Tile, LEVEL_WIDTH>, LEVEL_HEIGHT> tiles;
        Vec2i player;
        bool isWin;
    };

    std::array<std::array<Tile, LEVEL_WIDTH>, LEVEL_HEIGHT> m_tiles;
    Vec2i m_player; // index into m_tiles

    int m_levelNum = 1;
    bool m_isWin = false;

    std::array<GameState, MAX_HISTORY> m_history;
    size_t m_historyStart = 0; // oldest saved
    size_t m_historyCount = 0; // how many valid snapshots
};

} // namespace BabaIsYou