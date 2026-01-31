#include <raylib.h>
#include <array>
#include <cassert>
#include <string>
#include <vector>

constexpr int TILE_SIZE = 48;
constexpr int LEVEL_WIDTH = 33;
constexpr int LEVEL_HEIGHT = 18;
constexpr int SCREEN_WIDTH = TILE_SIZE * LEVEL_WIDTH;
constexpr int SCREEN_HEIGHT = TILE_SIZE * LEVEL_HEIGHT;

struct Vec2i {
    int x;
    int y;
};

// ' ' = empty
// '#' = wall
// 'B' = rock
// 'F' = flag
// '@' = player start

// clang-format off
static const std::vector<std::array<std::array<char, LEVEL_WIDTH + 1>, LEVEL_HEIGHT>> Levels{
{
    "#################################",
    "#       @                       #",
    "#           B B                 #",
    "#                               #",
    "#      F          ##            #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#################################",
}, {
    "#################################",
    "#       @                       #",
    "#           B B                 #",
    "#                               #",
    "#      F          ##            #",
    "#                               #",
    "#                               #",
    "#    BBBBBBBBBBBBBBBBBBB        #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#                               #",
    "#################################",
}
};
// clang-format on

enum Tile { EMPTY, WALL };

class Game {
  public:
    Game() {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sokoban");
        SetTargetFPS(60);
        LoadLevel(m_levelNum);
    }

    ~Game() { CloseWindow(); }

    void Update() {
        if (IsKeyPressed(KEY_W)) {
            TryMove(0, -1);
        } else if (IsKeyPressed(KEY_S)) {
            TryMove(0, 1);
        } else if (IsKeyPressed(KEY_A)) {
            TryMove(-1, 0);
        } else if (IsKeyPressed(KEY_D)) {
            TryMove(1, 0);
        }

        if (IsKeyPressed(KEY_R)) {
            LoadLevel(m_levelNum); // reset
        } else if (IsKeyPressed(KEY_N) && m_levelNum < Levels.size()) {
            LoadLevel(m_levelNum + 1);
        } else if (IsKeyPressed(KEY_P) && m_levelNum > 1) {
            LoadLevel(m_levelNum - 1);
        }
    }

    void Draw() const {
        BeginDrawing();
        ClearBackground({ 20, 20, 20, 255 });

        DrawGrid();
        DrawRocks();
        DrawFlags();
        DrawPlayer();

        if (m_isWin) {
            DrawText("You Win!", 50, 50, 50, GREEN);
        }

        EndDrawing();
    }

  private:
    void LoadLevel(int number) {
        number--;

        m_rocks.clear();
        m_flags.clear();
        m_player = { 0, 0 };
        m_isWin = false;

        bool isTherePlayer = false;
        bool isThereFlag = false;

        for (int y = 0; y < LEVEL_HEIGHT; ++y) {
            for (int x = 0; x < LEVEL_WIDTH; ++x) {
                m_level[y][x] = EMPTY;

                switch (Levels[number][y][x]) {
                    case 'B': m_rocks.emplace_back(x, y); break;
                    case 'F':
                        m_flags.emplace_back(x, y);
                        isThereFlag = true;
                        break;
                    case '@':
                        m_player = { x, y };
                        isTherePlayer = true;
                        break;
                    case '#': m_level[y][x] = WALL; break;
                    case ' ': break;
                    default: assert(0);
                }
            }
        }

        assert(isThereFlag);
        assert(isTherePlayer);
    }

    int RockIndexAt(int x, int y) const {
        for (int i = 0; i < m_rocks.size(); ++i) {
            if (m_rocks[i].x == x && m_rocks[i].y == y) {
                return i;
            }
        }
        return -1;
    }

    int FlagIndexAt(int x, int y) const {
        for (int i = 0; i < m_flags.size(); ++i) {
            if (m_flags[i].x == x && m_flags[i].y == y) {
                return i;
            }
        }
        return -1;
    }

    static bool InBounds(int x, int y) { return x >= 0 && x < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT; }

    void TryMove(int dx, int dy) {
        int nx = m_player.x + dx;
        int ny = m_player.y + dy;

        if (!InBounds(nx, ny)) { // off-screen
            return;
        }
        if (m_level[ny][nx] == WALL) {
            return; // can't move into wall
        }
        if (FlagIndexAt(nx, ny) != -1) {
            m_isWin = true;
        }

        int rockIndex = RockIndexAt(nx, ny);
        if (rockIndex == -1) {
            // empty -> move player
            m_player.x = nx;
            m_player.y = ny;
            return;
        }

        int bx = nx + dx;
        int by = ny + dy;

        while (InBounds(bx, by)) {
            if (m_level[by][bx] == WALL) {
                return;
            }
        }

        if (!InBounds(bx, by)) { // off-screen
            return;
        }

        if (m_level[by][bx] == WALL || RockIndexAt(bx, by) != -1) {
            return;
        }

        // push rock and move player
        if (rockIndex >= 0 && (size_t)rockIndex < m_rocks.size()) {
            m_rocks[rockIndex].x = bx;
            m_rocks[rockIndex].y = by;
        }

        m_player.x = nx;
        m_player.y = ny;
    }

    void DrawGrid() const {
        for (int y = 0; y < LEVEL_HEIGHT; ++y) {
            for (int x = 0; x < LEVEL_WIDTH; ++x) {
                Rectangle r = { (float)(x * TILE_SIZE), (float)(y * TILE_SIZE), (float)TILE_SIZE,
                    (float)TILE_SIZE };

                Color cellColor = m_level[y][x] == WALL ? DARKGRAY : Color{ 50, 50, 50, 255 };
                DrawRectangleRec(r, cellColor);

                // grid lines (cast to ints for the lines version)
                DrawRectangleLines((int)r.x, (int)r.y, (int)r.width, (int)r.height, Color{ 30, 30, 30, 255 });
            }
        }
    }

    void DrawRocks() const {
        for (const auto& b : m_rocks) {
            int px = b.x * TILE_SIZE;
            int py = b.y * TILE_SIZE;

            Rectangle rect{ px + 4.0f, py + 4.0f, TILE_SIZE - 8, TILE_SIZE - 8 };
            DrawRectangleRounded(rect, 0.3f, 6, Color{ 150, 100, 60, 255 });
        }
    }

    void DrawFlags() const {
        for (const auto& flag : m_flags) {
            int px = flag.x * TILE_SIZE;
            int py = flag.y * TILE_SIZE;

            DrawRectangle(px + 15, py + 5, TILE_SIZE - 42, TILE_SIZE - 10, YELLOW);
            DrawRectangle(px + 21, py + 5, 17, 16, YELLOW);
        }
    }

    void DrawPlayer() const {
        int px = m_player.x * TILE_SIZE;
        int py = m_player.y * TILE_SIZE;
        DrawRectangle(px + 6, py + 6, TILE_SIZE - 12, TILE_SIZE - 12, BLUE);

        // eyes
        DrawRectangle(px + 13, py + 15, 7, 7, BLACK);
        DrawRectangle(px + TILE_SIZE - 20, py + 15, 7, 7, BLACK);
    }

  private:
    std::array<std::array<Tile, LEVEL_WIDTH>, LEVEL_HEIGHT> m_level;
    Vec2i m_player;
    std::vector<Vec2i> m_rocks;
    std::vector<Vec2i> m_flags;

    std::vector<int> m_undoStack;

    int m_levelNum = 1;
    bool m_isWin = false;
};

int main() {
    Game game;

    while (!WindowShouldClose()) {
        game.Update();
        game.Draw();
    }

    return 0;
}
