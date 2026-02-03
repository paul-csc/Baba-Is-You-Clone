#pragma once

#include <array>

namespace BabaIsYou {

constexpr int LEVEL_WIDTH = 33;
constexpr int LEVEL_HEIGHT = 18;
constexpr int NUM_LEVEL = 2;

const std::array<std::array<char, LEVEL_WIDTH + 1>, LEVEL_HEIGHT>& GetLevel(int index);

} // namespace BabaIsYou
