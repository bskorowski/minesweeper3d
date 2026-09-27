#pragma once

#include "debug.hpp"
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <limits>
#include <numbers>

namespace math {

namespace internal {

// We have to do it like that isntead throwing in the func directly to not
// produce the warning about throwing in noexcept func.
// This should be used only as an "assert" in constexpr contexts.
// TODO :: Maybe extending the ASSERT() for that would actually be better.
constexpr void compileTimeError(const char *msg) { throw msg; }

constexpr auto factorialsLookupMap = []() {
  std::array<std::uint64_t, 21> t{1};
  for (int i = 1; i < 21; ++i) {
    t[i] = t[i - 1] * i;
  }
  return t;
}();

// TODO :: Might just be better to use std::factorial for c++26 once its
// constexpr
[[nodiscard]] constexpr std::uint64_t factorial(std::uint32_t x) noexcept {
  if consteval {
    if (x > 20) {
      compileTimeError("Maximum factorial is 20!");
    }
  } else {
    ASSERT(x <= 20, "Maximum factorial is 20!. Got {}", x);
  }

  return factorialsLookupMap[x];
}

[[nodiscard]] constexpr float pow(float x, std::uint32_t power) noexcept {

  std::uint32_t currPow = 1;
  double out = 1.0f;

  double currValue = static_cast<double>(x);
  while (power > 0) {
    while (currPow * 2 <= power) {
      currPow = currPow * 2;
      currValue = currValue * currValue;
    }

    out = out * currValue;
    power = power - currPow;
    currValue = x;
    currPow = 1;
  }
  return static_cast<float>(out);
}

[[nodiscard]] constexpr double floor(double x) {
  if consteval {
    if (x < static_cast<double>(std::numeric_limits<int64_t>::min()) ||
        x > static_cast<double>(std::numeric_limits<int64_t>::max())) {
      return x;
    }
    double trunc = static_cast<double>(static_cast<int64_t>(x));
    return trunc > x ? trunc - 1.0 : trunc;

  } else {
    return std::floor(x);
  }
}

// TODO :: Might just be better to use std::cos for c++26 once its constexpr
// Definitely not the best implementation, but simple enough
// Naive taylor series implementation
[[nodiscard]] constexpr float cos_taylor(float fx) {

  constexpr double twoPI = std::numbers::pi * 2.0;
  double x = static_cast<double>(fx < 0.0f ? -fx : fx);

  // Normalize
  const double cycles = internal::floor(x / twoPI);

  x = x - twoPI * cycles;

  const double x2 = x * x;
  double term = 1.0;
  double sum = term;

  // Taylor series
  for (std::uint32_t i = 2; i <= 64; i += 2) {
    term = term * ((-x2) / (i * (i - 1)));
    sum += term;
  }
  return static_cast<float>(sum);
}

} // namespace internal

[[nodiscard]] constexpr float cos(float radians) {

  if consteval {
    return internal::cos_taylor(radians);
  }

  return std::cos(radians);
}

[[nodiscard]] constexpr float sin(float radians) {
  // moving by 3/2pi may lose some precision, but we accept it for now
  return math::cos(static_cast<float>(static_cast<double>(radians) +
                                      std::numbers::pi * 1.5));
}

} // namespace math
