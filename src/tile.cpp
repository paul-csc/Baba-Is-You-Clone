#include "tile.h"
#include <cassert>

namespace BabaIsYou {

void Tile::Push(ObjectType type) {
    if (m_numObjects >= MAX_OBJECT_PER_TILE) {
        return;
    }

    m_objects[m_numObjects++] = type;
}

ObjectType Tile::Pop() {
    assert(m_numObjects >= 1);
    return m_objects[--m_numObjects];
}

bool Tile::Remove(ObjectType type) {
    for (int i = 0; i < m_numObjects; ++i) {
        if (m_objects[i] == type) {
            for (int j = i; j < m_numObjects - 1; j++) {
                m_objects[j] = m_objects[j + 1];
            }

            m_numObjects--;
            return true;
        }
    }
    return false;
}

void Tile::Clear() {
    m_numObjects = 0;
}

bool Tile::IsEmpty() const {
    return m_numObjects == 0;
}

bool Tile::Contains(ObjectType type) const {
    for (int i = 0; i < m_numObjects; ++i) {
        if (m_objects[i] == type) {
            return true;
        }
    }
    return false;
}

bool Tile::Contains(const std::vector<ObjectType>& type) const {
    for (const auto t : type) {
        if (Contains(t)) {
            return true;
        }
    }
    return false;
}

} // namespace BabaIsYou
