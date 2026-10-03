#pragma once

#include "math/matrix.hpp"
#include "ui/components/modifier.hpp"
#include <imgui.h>

[[nodiscard]] auto measureText(std::string_view textToMeasure) noexcept -> v2f;

// Returns remaining available widtth.
// Effectively SreenWidth - currentDrawPos
[[nodiscard]] auto getAvailableRegion() noexcept -> v2f;
// Get the position that next thing will be drawn
[[nodiscard]] auto getCurrentDrawPosition() noexcept -> v2f;
// Set the position that next thing will be drawn
void setCurrentDrawPosition(v2f pos) noexcept;

void moveCursorBy(v2f size) noexcept;

void baseElement(const v2f contentSize, const Modifier &modifier,
                 auto &&drawContent) {
  std::vector<v2f> measurements(modifier.operations().size() + 1, contentSize);

  // Measure from
  for (size_t i = modifier.operations().size(); i-- > 0;) {

    const ModifierOp &op = modifier.operations().at(i);
    const v2f lastSize = measurements[i + 1];

    measurements[i] = std::visit(
        Visitor{[lastSize](const FillWidth &fill) {
                  return vec2<float>(getAvailableRegion().x() * fill.percentage,
                                     lastSize.y());
                },
                [lastSize](auto &&) { return lastSize; }},
        op);
  }

  v2f origin = getCurrentDrawPosition();
  v2f pos = origin;
  v2f box = measurements[0];
  for (size_t i = 0; i < modifier.operations().size(); ++i) {

    const ModifierOp &op = modifier.operations().at(i);
    std::visit(Visitor{[&](const AlignCenter &align) {
                         v2f child = measurements[i + 1];
                         pos.x() += (box.x() - child.x()) * align.alignment.x();
                         pos.y() += (box.y() - child.y()) * align.alignment.y();
                       },
                       [](auto &&) {}},
               op);
  }
  setCurrentDrawPosition(pos);
  drawContent();
  setCurrentDrawPosition(origin);
  moveCursorBy(measurements[0]);

  // We measure
}
