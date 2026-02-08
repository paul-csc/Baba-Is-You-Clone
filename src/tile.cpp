#include "tile.h"
#include "raylib.h"
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

void Tile::Draw(int x, int y) const {
    Rectangle r{ x * TILE_PIXEL_SIZE, y * TILE_PIXEL_SIZE, TILE_PIXEL_SIZE, TILE_PIXEL_SIZE };

    for (int i = 0; i < m_numObjects; i++) {
        auto object = m_objects[i];

        if (object == ObjectType::Wall) {
            DrawRectangleRec(r, DARKGRAY);
        } else if (object == ObjectType::Rock) {
            DrawRectangleRounded(
                { r.x + 7.0f, r.y + 7.0f, TILE_PIXEL_SIZE - 14, TILE_PIXEL_SIZE - 14 }, 0.3f, 6,
                { 150, 100, 60, 255 });
        } else if (object == ObjectType::Flag) {
            DrawRectangle(r.x + 15, r.y + 5, TILE_PIXEL_SIZE - 42, TILE_PIXEL_SIZE - 10, YELLOW);
            DrawRectangle(r.x + 21, r.y + 5, 17, 16, YELLOW);
        } else if (object == ObjectType::Baba) {
            DrawRectangleRec(
                { r.x + 6, r.y + 6, TILE_PIXEL_SIZE - 12, TILE_PIXEL_SIZE - 12 }, BLUE);

            // eyes
            DrawRectangleRec({ r.x + 13, r.y + 15, 7, 7 }, BLACK);
            DrawRectangleRec({ r.x + TILE_PIXEL_SIZE - 20, r.y + 15, 7, 7 }, BLACK);
        } else if (IsText(object)) {
            DrawRectangleRounded(
                { r.x + 6.0f, r.y + 6.0f, TILE_PIXEL_SIZE - 12, TILE_PIXEL_SIZE - 12 }, 0.3f, 3,
                GREEN);
            int length = MeasureText(TypeToStr(object).c_str(), 18);
            DrawText(TypeToStr(object).c_str(), r.x + (TILE_PIXEL_SIZE - length) / 2,
                r.y + TILE_PIXEL_SIZE / 2 - 9, 18, BLACK);
        }
    }
}

} // namespace BabaIsYou
