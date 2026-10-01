#include "input.hpp"
#include "GLFW/glfw3.h"
#include "render/window/window.hpp"
#include <utility>

using GLFWKeyCode = int;
using GLFWKeyState = int;
using GLFWMouseButton = int;

namespace {

[[nodiscard]] constexpr GLFWKeyCode keyToGLFW(Key k) noexcept {
  static constexpr auto map = []() {
    std::array<GLFWKeyCode, std::to_underlying(Key::__SizeGuard)> arr{};
    arr.fill(GLFW_KEY_UNKNOWN);

    arr[std::to_underlying(Key::A)] = GLFW_KEY_A;
    arr[std::to_underlying(Key::B)] = GLFW_KEY_B;
    arr[std::to_underlying(Key::C)] = GLFW_KEY_C;
    arr[std::to_underlying(Key::D)] = GLFW_KEY_D;
    arr[std::to_underlying(Key::E)] = GLFW_KEY_E;
    arr[std::to_underlying(Key::F)] = GLFW_KEY_F;
    arr[std::to_underlying(Key::G)] = GLFW_KEY_G;
    arr[std::to_underlying(Key::H)] = GLFW_KEY_H;
    arr[std::to_underlying(Key::I)] = GLFW_KEY_I;
    arr[std::to_underlying(Key::J)] = GLFW_KEY_J;
    arr[std::to_underlying(Key::K)] = GLFW_KEY_K;
    arr[std::to_underlying(Key::L)] = GLFW_KEY_L;
    arr[std::to_underlying(Key::M)] = GLFW_KEY_M;
    arr[std::to_underlying(Key::N)] = GLFW_KEY_N;
    arr[std::to_underlying(Key::O)] = GLFW_KEY_O;
    arr[std::to_underlying(Key::P)] = GLFW_KEY_P;
    arr[std::to_underlying(Key::Q)] = GLFW_KEY_Q;
    arr[std::to_underlying(Key::R)] = GLFW_KEY_R;
    arr[std::to_underlying(Key::S)] = GLFW_KEY_S;
    arr[std::to_underlying(Key::T)] = GLFW_KEY_T;
    arr[std::to_underlying(Key::U)] = GLFW_KEY_U;
    arr[std::to_underlying(Key::V)] = GLFW_KEY_V;
    arr[std::to_underlying(Key::W)] = GLFW_KEY_W;
    arr[std::to_underlying(Key::X)] = GLFW_KEY_X;
    arr[std::to_underlying(Key::Y)] = GLFW_KEY_Y;
    arr[std::to_underlying(Key::Z)] = GLFW_KEY_Z;
    arr[std::to_underlying(Key::Space)] = GLFW_KEY_SPACE;
    arr[std::to_underlying(Key::LeftControl)] = GLFW_KEY_LEFT_CONTROL;
    arr[std::to_underlying(Key::LeftAlt)] = GLFW_KEY_LEFT_ALT;
    arr[std::to_underlying(Key::Escape)] = GLFW_KEY_ESCAPE;
    arr[std::to_underlying(Key::F1)] = GLFW_KEY_F1;

    return arr;
  }();

  const auto index = std::to_underlying(k);
  if (index >= map.size())
    return GLFW_KEY_UNKNOWN;
  ASSERT(map[index] != GLFW_KEY_UNKNOWN,
         "Mapping should exist for key (int): {}", std::to_underlying(k));
  return map[index];
}

[[nodiscard]] constexpr GLFWMouseButton
mouseButtonToGLFW(MouseButton button) noexcept {

  static constexpr auto map = []() {
    std::array<GLFWMouseButton, std::to_underlying(MouseButton::__SizeGuard)>
        arr{};
    arr.fill(GLFW_KEY_UNKNOWN);
    arr[std::to_underlying(MouseButton::Left)] = GLFW_MOUSE_BUTTON_LEFT;
    arr[std::to_underlying(MouseButton::Right)] = GLFW_MOUSE_BUTTON_RIGHT;
    return arr;
  }();

  const auto index = std::to_underlying(button);
  if (index >= map.size())
    return GLFW_KEY_UNKNOWN;
  ASSERT(map[index] != GLFW_KEY_UNKNOWN, "Mapping should exist");
  return map[index];
}
} // namespace

void Input::update(const Window &window) noexcept {

  // Updating keyboard
  constexpr size_t keyCount = std::to_underlying(Key::__SizeGuard);
  for (size_t i = 0; i < keyCount; ++i) {
    Key k = static_cast<Key>(i);
    GLFWKeyCode GLFWcode = keyToGLFW(k);

    if (GLFWcode != GLFW_KEY_UNKNOWN) {
      bool isDown = glfwGetKey(window.getHandle(), GLFWcode) == GLFW_PRESS;

      ASSERT(glfwGetKey(window.getHandle(), GLFWcode) == GLFW_PRESS ||
                 glfwGetKey(window.getHandle(), GLFWcode) == GLFW_RELEASE,
             "glfwGetKey 'should' have only two states");

      keyStates[i] = getState(keyStates[i], isDown);
    }
  }

  // Updating mouse click states
  constexpr auto mouseButtons = std::to_underlying(MouseButton::__SizeGuard);
  for (std::uint8_t i = 0; i < mouseButtons; ++i) {
    MouseButton b = static_cast<MouseButton>(i);
    GLFWMouseButton GLFWcode = mouseButtonToGLFW(b);

    if (GLFWcode != GLFW_KEY_UNKNOWN) {
      bool isDown =
          glfwGetMouseButton(window.getHandle(), GLFWcode) == GLFW_PRESS;

      ASSERT(glfwGetMouseButton(window.getHandle(), GLFWcode) == GLFW_PRESS ||
                 glfwGetMouseButton(window.getHandle(), GLFWcode) ==
                     GLFW_RELEASE,
             "glfwGetKey 'should' have only two states");

      mouseStates[i] = getState(mouseStates[i], isDown);
    }
  }

  // Updating mouse position
  lastMousePosition = mousePosition;
  glfwGetCursorPos(window.getHandle(), &(mousePosition.data[0][0]),
                   &(mousePosition.data[0][1]));
}
