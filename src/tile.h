#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace BabaIsYou {

constexpr size_t MAX_OBJECT_PER_TILE = 5;

enum class ObjectType { Wall, Baba, Flag, Rock };
enum class Property { You, Stop, Win, Push };

class Tile {
  public:
    void Push(ObjectType type);
    ObjectType Pop();
    bool Remove(ObjectType type);
    void Clear();
    bool IsEmpty() const;
    bool Contains(ObjectType type) const;
    bool Contains(const std::vector<ObjectType>& types) const;

    auto begin() const { return m_objects.begin(); }
    auto end() const { return m_objects.begin() + m_numObjects; }

  private:
    std::array<ObjectType, MAX_OBJECT_PER_TILE> m_objects;
    int m_numObjects = 0;
};

} // namespace BabaIsYou
