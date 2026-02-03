#pragma once

#include <array>
#include <cstddef>

namespace BabaIsYou {

constexpr size_t MAX_OBJECT_PER_TILE = 6;

enum class TileType { Empty, Wall, Player, Flag, Rock, Rule, NumTileType };

enum class RuleType {
    Baba,
    Is,
    You,
    Wall,
    Stop,
    Rock,
    Push,
    Flag,
    Win,
};

class Tile {
  public:
    void Push(TileType type);
    TileType Pop();
    bool Remove(TileType type);
    bool IsEmpty() const;
    bool Contains(TileType type) const;

    // iteration (non-const)
    auto begin() { return m_objects.begin(); }
    auto end() { return m_objects.begin() + m_numObjects; }

    // iteration (const)
    auto begin() const { return m_objects.begin(); }
    auto end() const { return m_objects.begin() + m_numObjects; }

  private:
    std::array<TileType, MAX_OBJECT_PER_TILE> m_objects;
    int m_numObjects = 0;
};

} // namespace BabaIsYou