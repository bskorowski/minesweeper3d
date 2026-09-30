#pragma once

#include "debug.hpp"
#include "utility/cast.hpp"
#include <algorithm>
#include <numeric>
#include <random>

/**
 * Generates a vector of unique random numbers from inclusive range [min,max]
 */
template <std::integral IntType>
[[nodiscard]] constexpr std::vector<IntType>
randomUniqueRange(IntType min, IntType max) noexcept {

  ASSERT(min <= max);

  std::random_device seed;
  std::mt19937 rng{seed()};

  // Creating the vector filled with sorted range
  std::vector<IntType> out(max - min + 1);
  std::iota(out.begin(), out.end(), min);

  // Shuffling the sorted range
  std::shuffle(out.begin(), out.end(), rng);

  return out;
}
