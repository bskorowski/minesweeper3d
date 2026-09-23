#include "input.hpp"
#include "GLFW/glfw3.h"
#include "render/window/window.hpp"

using GLFWKeyCode = int;
using GLFWKeyState = int;
using GLFWMouseButton = int;

namespace {

[[nodiscard]] constexpr GLFWKeyCode keyToGLFW(Key k) noexcept {
  static constexpr auto map = []() {
    std::array<GLFWKeyCode, static_cast<size_t>(Key::__SizeGuard)> arr{};
    arr.fill(GLFW_KEY_UNKNOWN);

    arr[static_cast<size_t>(Key::A)] = GLFW_KEY_A;
    arr[static_cast<size_t>(Key::B)] = GLFW_KEY_B;
    arr[static_cast<size_t>(Key::C)] = GLFW_KEY_C;
    arr[static_cast<size_t>(Key::D)] = GLFW_KEY_D;
    arr[static_cast<size_t>(Key::E)] = GLFW_KEY_E;
    arr[static_cast<size_t>(Key::F)] = GLFW_KEY_F;
    arr[static_cast<size_t>(Key::G)] = GLFW_KEY_G;
    arr[static_cast<size_t>(Key::H)] = GLFW_KEY_H;
    arr[static_cast<size_t>(Key::I)] = GLFW_KEY_I;
    arr[static_cast<size_t>(Key::J)] = GLFW_KEY_J;
    arr[static_cast<size_t>(Key::K)] = GLFW_KEY_K;
    arr[static_cast<size_t>(Key::L)] = GLFW_KEY_L;
    arr[static_cast<size_t>(Key::M)] = GLFW_KEY_M;
    arr[static_cast<size_t>(Key::N)] = GLFW_KEY_N;
    arr[static_cast<size_t>(Key::O)] = GLFW_KEY_O;
    arr[static_cast<size_t>(Key::P)] = GLFW_KEY_P;
    arr[static_cast<size_t>(Key::Q)] = GLFW_KEY_Q;
    arr[static_cast<size_t>(Key::R)] = GLFW_KEY_R;
    arr[static_cast<size_t>(Key::S)] = GLFW_KEY_S;
    arr[static_cast<size_t>(Key::T)] = GLFW_KEY_T;
    arr[static_cast<size_t>(Key::U)] = GLFW_KEY_U;
    arr[static_cast<size_t>(Key::V)] = GLFW_KEY_V;
    arr[static_cast<size_t>(Key::W)] = GLFW_KEY_W;
    arr[static_cast<size_t>(Key::X)] = GLFW_KEY_X;
    arr[static_cast<size_t>(Key::Y)] = GLFW_KEY_Y;
    arr[static_cast<size_t>(Key::Z)] = GLFW_KEY_Z;
    arr[static_cast<size_t>(Key::Space)] = GLFW_KEY_SPACE;
    arr[static_cast<size_t>(Key::LeftControl)] = GLFW_KEY_LEFT_CONTROL;
    arr[static_cast<size_t>(Key::LeftAlt)] = GLFW_KEY_LEFT_ALT;
    arr[static_cast<size_t>(Key::Escape)] = GLFW_KEY_ESCAPE;
    arr[static_cast<size_t>(Key::F1)] = GLFW_KEY_F1;

    return arr;
  }();

  const auto index = static_cast<size_t>(k);
  if (index >= map.size())
    return GLFW_KEY_UNKNOWN;
  ASSERT(map[index] != GLFW_KEY_UNKNOWN,
         std::format("Mapping should exist for key (int): {}",
                     static_cast<size_t>(k)));
  return map[index];
}

[[nodiscard]] constexpr GLFWMouseButton
mouseButtonToGLFW(MouseButton button) noexcept {

  static constexpr auto map = []() {
    std::array<GLFWMouseButton, static_cast<size_t>(MouseButton::__SizeGuard)>
        arr{};
    arr.fill(GLFW_KEY_UNKNOWN);
    arr[static_cast<size_t>(MouseButton::Left)] = GLFW_MOUSE_BUTTON_LEFT;
    arr[static_cast<size_t>(MouseButton::Right)] = GLFW_MOUSE_BUTTON_RIGHT;
    return arr;
  }();

  const auto index = static_cast<size_t>(button);
  if (index >= map.size())
    return GLFW_KEY_UNKNOWN;
  ASSERT(map[index] != GLFW_KEY_UNKNOWN, "Mapping should exist");
  return map[index];
}
} // namespace

void Input::update(const Window &window) noexcept {

  // Updating keyboard
  constexpr size_t keyCount = static_cast<size_t>(Key::__SizeGuard);
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
  constexpr size_t mouseButtons = static_cast<size_t>(MouseButton::__SizeGuard);
  for (size_t i = 0; i < mouseButtons; ++i) {
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
