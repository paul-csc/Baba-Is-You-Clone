#include "game.h"
#include "raylib.h"
#include <cassert>

namespace BabaIsYou {

Game::Game() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sokoban");
    SetTargetFPS(60);
    LoadLevel(m_levelNum);
}

Game::~Game() {
    CloseWindow();
}

void Game::Loop() {
    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
}

void Game::Update() {
    if (!m_isWin) {
        if (IsKeyPressed(KEY_W)) {
            TryMove(0, -1);
        } else if (IsKeyPressed(KEY_S)) {
            TryMove(0, 1);
        } else if (IsKeyPressed(KEY_A)) {
            TryMove(-1, 0);
        } else if (IsKeyPressed(KEY_D)) {
            TryMove(1, 0);
        }
    }

    if (IsKeyPressed(KEY_R)) {
        LoadLevel(m_levelNum);
    } else if (IsKeyPressed(KEY_N) && m_levelNum < NUM_LEVEL) {
        LoadLevel(++m_levelNum);
    } else if (IsKeyPressed(KEY_P) && m_levelNum > 1) {
        LoadLevel(--m_levelNum);
    } else if (IsKeyPressed(KEY_X)) {
        Undo();
    }
}

void Game::Draw() const {
    BeginDrawing();
    ClearBackground({ 20, 20, 20, 255 });

    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            Rectangle r = { x * TILE_PIXEL_SIZE, y * TILE_PIXEL_SIZE, TILE_PIXEL_SIZE,
                TILE_PIXEL_SIZE };

            DrawRectangleRec(r, { 50, 50, 50, 255 });
            DrawRectangleLines(
                (int)r.x, (int)r.y, (int)r.width, (int)r.height, { 30, 30, 30, 255 });

            for (const auto object : m_tiles[y][x]) {
                if (object == TileType::Wall) {
                    DrawRectangleRec(r, DARKGRAY);
                } else if (object == TileType::Rock) {
                    DrawRectangleRounded(
                        { r.x + 4.0f, r.y + 4.0f, TILE_PIXEL_SIZE - 8, TILE_PIXEL_SIZE - 8 }, 0.3f,
                        6, { 150, 100, 60, 255 });
                } else if (object == TileType::Flag) {
                    DrawRectangle(
                        r.x + 15, r.y + 5, TILE_PIXEL_SIZE - 42, TILE_PIXEL_SIZE - 10, YELLOW);
                    DrawRectangle(r.x + 21, r.y + 5, 17, 16, YELLOW);
                } else if (object == TileType::Player) {
                    DrawRectangleRec(
                        { r.x + 6, r.y + 6, TILE_PIXEL_SIZE - 12, TILE_PIXEL_SIZE - 12 }, BLUE);

                    // eyes
                    DrawRectangleRec({ r.x + 13, r.y + 15, 7, 7 }, BLACK);
                    DrawRectangleRec({ r.x + TILE_PIXEL_SIZE - 20, r.y + 15, 7, 7 }, BLACK);
                }
            }
        }
    }

    if (m_isWin) {
        DrawText("You Win!", 50, 50, 50, GREEN);
    }

    EndDrawing();
}

bool Game::InBounds(int x, int y) {
    return x >= 0 && x < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT;
}

void Game::LoadLevel(int number) {
    number--;
    assert(number >= 0 && number < NUM_LEVEL);

    m_historyStart = 0;
    m_historyCount = 0;

    m_isWin = false;
    bool hasPlayer = false;
    bool hasFlag = false;

    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            m_tiles[y][x] = Tile{};

            switch (GetLevel(number)[y][x]) {
                case '#': m_tiles[y][x].Push(TileType::Wall); break;
                case 'R': m_tiles[y][x].Push(TileType::Rock); break;
                case 'F':
                    m_tiles[y][x].Push(TileType::Flag);
                    hasFlag = true;
                    break;
                case '@':
                    m_tiles[y][x].Push(TileType::Player);
                    m_player = { x, y };
                    hasPlayer = true;
                    break;
                case ' ': break;
                default: assert(false);
            }
        }
    }

    assert(hasPlayer);
    assert(hasFlag);

    SaveState();
}

void Game::TryMove(int dx, int dy) {
    const int nx = m_player.x + dx;
    const int ny = m_player.y + dy;

    if (!InBounds(nx, ny) || m_tiles[ny][nx].Contains(TileType::Wall)) {
        return;
    }

    // scan forward until non-rock
    int cx = nx;
    int cy = ny;
    while (InBounds(cx, cy) && m_tiles[cy][cx].Contains(TileType::Rock)) {
        cx += dx;
        cy += dy;
    }

    if (!InBounds(cx, cy) || m_tiles[cy][cx].Contains(TileType::Wall)) {
        return;
    }

    // if destination cell already has a rock (shouldn't happen since we scanned), block
    // but scanning should stop at first non-rock, so it's safe.

    SaveState();

    // shift rocks forward (back-to-front)
    while (cx != nx || cy != ny) {
        const int px = cx - dx;
        const int py = cy - dy;

        // if there is a rock at source, move it to destination
        if (m_tiles[py][px].Contains(TileType::Rock)) {
            m_tiles[cy][cx].Push(TileType::Rock);
            m_tiles[py][px].Remove(TileType::Rock);
        }

        cx = px;
        cy = py;
    }

    // move player
    m_tiles[m_player.y][m_player.x].Remove(TileType::Player);
    m_tiles[ny][nx].Push(TileType::Player);
    m_player.x = nx;
    m_player.y = ny;

    if (m_tiles[ny][nx].Contains(TileType::Flag)) {
        m_isWin = true;
    }
}

void Game::SaveState() {
    size_t index = (m_historyStart + m_historyCount) % MAX_HISTORY;

    m_history[index] = GameState(m_tiles, m_player, m_isWin);

    if (m_historyCount < MAX_HISTORY) {
        m_historyCount++;
    } else {
        m_historyStart = (m_historyStart + 1) % MAX_HISTORY;
    }
}

void Game::Undo() {
    if (m_historyCount == 0) {
        return;
    }

    size_t index = (m_historyStart + m_historyCount - 1) % MAX_HISTORY;

    m_tiles = m_history[index].tiles;
    m_player = m_history[index].player;
    m_isWin = m_history[index].isWin;

    m_historyCount--;
}

} // namespace BabaIsYou