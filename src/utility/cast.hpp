#pragma once

#include "debug.hpp"
#include "utility/types.hpp"
#include <cmath>
#include <concepts>
#include <limits>
#include <utility>

// Only check if integer part doesn't change after conversion,
// so for example for double->float 0.1 representation differs, but it is
// considered acceptable
template <typename Target, typename Source>
  requires((Numeric<Source>) && (Numeric<Target>))
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

// General purpose cast, designed to be the most common type of cast.
template <class Target, class Source>
constexpr auto cast(Source &&source) noexcept -> decltype(auto) {

  static_assert(!isSameType<Source, Target>(),
                "Trying to cast type to itself is useless");

  if constexpr (Numeric<Target> && Numeric<Source>) {
    ASSERT(inRange<Target>(source), "Value of type '{}({})' outside of {}({}) ",
           typeid(Source).name(), source, typeid(Target).name(),
           static_cast<Target>(source));
  }

  return static_cast<Target>(std::forward<Source>(source));
}

// Special type of casting, allowing casting to the same type.
// Most useful in templates, or other forms of code, where casting to the same
// type does not necessarily mean an error
template <class Target, class Source>
constexpr auto castOrSame(Source &&source) noexcept -> decltype(auto) {

  if constexpr (isSameType<Source, Target>()) {
    return std::forward<Source>(source);
  } else {
    return cast<Target>(std::forward<Source>(source));
  }
}
