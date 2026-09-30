#pragma once

#include "debug.hpp"
#include <cmath>
#include <concepts>
#include <limits>
#include <utility>

// Only check if integer part doesn't change after conversion,
// so for example for double->float 0.1 representation differs, but it is
// considered acceptable
template <typename Target, typename Source>
  requires((std::integral<Target> || std::floating_point<Target>) &&
           (std::integral<Source> || std::floating_point<Source>))
constexpr bool inRange(Source value) noexcept {

  using SourceLimit = std::numeric_limits<Source>;
  using TargetLimit = std::numeric_limits<Target>;

  if constexpr (std::integral<Source> && std::integral<Target>) {
    return std::in_range<Target>(value);
  } else if constexpr (std::floating_point<Source> &&
                       std::floating_point<Target>) {
    // NaN and inf convert fine
    if (std::isnan(value) || std::isinf(value)) {
      return true;
    }

    // Wider is fine
    if constexpr (SourceLimit::max_exponent <= TargetLimit::max_exponent &&
                  SourceLimit::min_exponent >= TargetLimit::min_exponent &&
                  SourceLimit::digits <= TargetLimit::digits) {
      return true;
    }

    // Make sure value is not out of target's max
    return std::abs(value) <= static_cast<Source>(TargetLimit::max());
  } else if constexpr (std::floating_point<Source> && std::integral<Target>) {
    // truncate towards 0
    value = std::trunc(value);

    // 0 or -2^k, representable in float, safe to cast
    constexpr Source low = static_cast<Source>(TargetLimit::min());
    // max is (2 pow k) - 1, which would get rounded up if represented as float
    // Make sure it is (2 pow k), so it is easily representable, then compare as
    // '<'
    constexpr Source high = static_cast<Source>(TargetLimit::max() / 2 + 1) * 2;

    return low <= value && value < high;
  } else if constexpr (std::integral<Source> && std::floating_point<Target>) {
    Target converted = static_cast<Target>(value);
    // make sure converting back is not UB, then compare if value same
    return inRange<Source>(converted) &&
           value == static_cast<Source>(converted);
  } else {
    static_assert(false, "Unknown type combination");
  }
}

template <typename Target, typename Source>
  requires((std::integral<Target> || std::floating_point<Target>) &&
           (std::integral<Source> || std::floating_point<Source>))
constexpr Target cast(Source value) noexcept {
  static_assert(!std::same_as<Source, Target>,
                "Trying to cast type to itself is useless");
  ASSERT(inRange<Target>(value), "Value of type '{}({})' outside of {}({})",
         typeid(Source).name(), value, typeid(Target).name(),
         static_cast<Target>(value));
  return static_cast<Target>(value);
}
