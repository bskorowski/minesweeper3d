#pragma once

#include <concepts>

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <typename T, typename U> constexpr auto isSameType() noexcept -> bool {
  return std::same_as<T, U>;
}

template <class... Visitors> struct Visitor : Visitors... {
  using Visitors::operator()...;
};
