#pragma once
#include "math/matrix.hpp"
#include <variant>
#include <vector>

#define ASSERT_PERCENTAGE(name, value)                                         \
  ASSERT(value >= .0f && value <= 1.f,                                         \
         #value " " #name " must be in [0;1] range. Got '{}'", value)

struct AlignCenter {
  // 0..1
  v2f alignment;

  constexpr AlignCenter(float x, float y) noexcept
      : alignment({vec2<float>(x, y)}) {
    ASSERT_PERCENTAGE(alignment, x);
    ASSERT_PERCENTAGE(alignment, y);
  }
};

struct FillWidth {
  float percentage;

  constexpr FillWidth(float percentage) noexcept : percentage{percentage} {
    ASSERT_PERCENTAGE(width fill percentage, percentage);
  }
};

using ModifierOp = std::variant<AlignCenter, FillWidth>;

struct Modifier {

public:
  constexpr Modifier() noexcept { ops_.reserve(8); }

  constexpr auto centerHorizontally() && noexcept -> Modifier;
  constexpr auto fillWidth(float percentage = 1.0f) && noexcept -> Modifier;

  constexpr auto operations() const noexcept -> const std::vector<ModifierOp> &;

private:
  // TODO :: Modifier could te templated and hold constant amount of
  // arguments, as they maximum amount is known when creating them.
  // std::array<ModifierOp,Size> ops_;
  std::vector<ModifierOp> ops_;
};

constexpr auto Modifier::centerHorizontally() && noexcept -> Modifier {
  ops_.emplace_back(AlignCenter{0.5f, 0.0f});
  return std::move(*this);
}

constexpr auto Modifier::fillWidth(float percentage) && noexcept -> Modifier {
  ops_.emplace_back(FillWidth{percentage});
  return std::move(*this);
}

constexpr auto Modifier::operations() const noexcept
    -> const std::vector<ModifierOp> & {
  return ops_;
}
