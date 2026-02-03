#include "level.h"
#include <cassert>

namespace BabaIsYou {

namespace {

/*
 ' ' = empty
 '#' = wall
 'R' = rock
 'F' = flag
 '@' = player start
*/

// clang-format off
const std::array<std::array<std::array<char, LEVEL_WIDTH + 1>, LEVEL_HEIGHT>, NUM_LEVEL> Levels {{
{
    "#################################",
    "#       @                       #",
    "#           R R                 #",
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
    "#           RRRRRRRR            #",
    "#                               #",
    "#      F          ##            #",
    "#                               #",
    "#                               #",
    "#    RRRRRRRRRRRRRRRRRRR        #",
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
}};
// clang-format on

} // namespace

const std::array<std::array<char, LEVEL_WIDTH + 1>, LEVEL_HEIGHT>& GetLevel(int index) {
    assert(index < NUM_LEVEL && index >= 0);
    return Levels[index];
}

} // namespace BabaIsYou
