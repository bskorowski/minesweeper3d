#include "components.hpp"
#include "ui/components/core.hpp"
#include <imgui.h>
#include <logzy/logzy.hpp>

namespace components {
void Text(std::string_view text, Modifier modifier) {
  baseElement(measureText(text), modifier, [&]() {
    ImGui::TextUnformatted(text.data(), text.data() + text.length());
  });
}

void Text(std::string_view text, TextStyle style, Modifier modifier) {
  ImGui::PushFont(nullptr, style.fontSize_);
  baseElement(measureText(text), modifier, [&]() {
    ImGui::TextUnformatted(text.data(), text.data() + text.length());
  });
  ImGui::PopFont();
}
} // namespace components
