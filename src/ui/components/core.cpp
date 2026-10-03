#include "core.hpp"
#include "imgui.h"
#include "ui/components/modifier.hpp"
#include <variant>

[[nodiscard]] auto measureText(std::string_view textToMeasure) noexcept -> v2f {
  return ImGui::CalcTextSize(textToMeasure.data(),
                             textToMeasure.data() + textToMeasure.length());
}

// Returns remaining available widtth.
// Effectively SreenWidth - currentDrawPos
[[nodiscard]] auto getAvailableRegion() noexcept -> v2f {
  return ImGui::GetContentRegionAvail();
}

[[nodiscard]] auto getCurrentDrawPosition() noexcept -> v2f {
  return ImGui::GetCursorScreenPos();
}

void setCurrentDrawPosition(v2f pos) noexcept {
  ImGui::SetCursorScreenPos(pos);
}

void moveCursorBy(v2f size) noexcept { ImGui::Dummy(size); }
