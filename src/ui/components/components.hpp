#pragma once

#include "modifier.hpp"
#include <string_view>

struct TextStyle {
  float fontSize_;

  [[nodiscard]] constexpr auto fontSize(float size) && {
    fontSize_ = size;
    return std::move(*this);
  }
};

namespace components {

// Normal text
void Text(std::string_view text, Modifier modifier = Modifier{});
void Text(std::string_view text, TextStyle style,
          Modifier modifier = Modifier{});
} // namespace components
