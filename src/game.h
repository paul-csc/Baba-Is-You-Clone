#pragma once

#include "bimap.h"
#include "level.h"
#include "tile.h"
#include <string>
#include <vector>

namespace BabaIsYou {

constexpr float TILE_PIXEL_SIZE = 48.0f;
constexpr int SCREEN_WIDTH = TILE_PIXEL_SIZE * LEVEL_WIDTH;
constexpr int SCREEN_HEIGHT = TILE_PIXEL_SIZE * LEVEL_HEIGHT;
constexpr size_t MAX_HISTORY = 512;

class Game {
  public:
    Game();
    ~Game();

    void Loop();

  private:
    void Update();
    void Draw() const;

    void LoadLevel(int number);

    static bool InBounds(int x, int y);
    static bool VecContains(const std::vector<ObjectType>& v, ObjectType type);
    bool AllPushable(const Tile& tile, const std::vector<ObjectType>& pushObjects) const;
    void TryMove(int dx, int dy);

    void SaveState();
    void LoadState(const GameState& gs);
    void Undo();

    std::array<std::array<Tile, LEVEL_WIDTH>, LEVEL_HEIGHT> m_tiles;

    int m_levelNum = 1;
    bool m_isWin = false;

    BiMap<ObjectType, Property> m_rules;

    std::array<GameState, MAX_HISTORY> m_history;
    size_t m_historyStart = 0; // oldest saved
    size_t m_historyCount = 0; // how many valid snapshots
};

} // namespace BabaIsYou