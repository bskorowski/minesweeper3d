#pragma once

#include "debug.hpp"
#include "math/matrix.hpp"
#include <array>
#include <stdint.h>
#include <utility>

struct Window;

enum class KeyState : std::uint8_t {
  // Key is up
  Idle = 0,
  // Key was idle and got just pressed
  Pressed = 1,
  // Key was pressed on previous frame and is still pressed
  Held = 2,
  // Key was pressed and just got released
  Released = 3
};

enum class Key : std::uint8_t {
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  Space,
  LeftControl,
  LeftAlt,
  Escape,
  F1,
  __SizeGuard,
  Unknown,
};

[[nodiscard]] constexpr KeyState getState(KeyState last, bool isDown) noexcept {

  if (isDown) {
    if (last == KeyState::Idle || last == KeyState::Released) {
      return KeyState::Pressed;
    }
    if (last == KeyState::Pressed || last == KeyState::Held) {
      return KeyState::Held;
    }
  } else {
    if (last == KeyState::Idle || last == KeyState::Released) {
      return KeyState::Idle;
    }
    if (last == KeyState::Pressed || last == KeyState::Held) {
      return KeyState::Released;
    }
  }

  ASSERT(false, "All paths should be covered");
  std::unreachable();
}

enum class MouseButton : std::uint8_t { Left, Right, __SizeGuard };

struct Input {
  constexpr Input() noexcept : keyStates{}, mouseStates{} {}

  Input(const Input &other) = delete;
  Input(Input &&other) = delete;
  Input &operator=(Input &&other) = delete;
  Input &operator=(const Input &other) = delete;

  void update(const Window &window) noexcept;

  [[nodiscard]] constexpr bool isPressed(Key k) const noexcept {
    return keyStates[std::to_underlying(k)] == KeyState::Pressed;
  }

  [[nodiscard]] constexpr bool isReleased(Key k) const noexcept {
    return keyStates[std::to_underlying(k)] == KeyState::Released;
  }

  [[nodiscard]] constexpr bool isHeld(Key k) const noexcept {
    return keyStates[std::to_underlying(k)] == KeyState::Held;
  }

  [[nodiscard]] constexpr bool isUp(Key k) const noexcept {
    return keyStates[std::to_underlying(k)] == KeyState::Idle ||
           keyStates[std::to_underlying(k)] == KeyState::Released;
  }

  [[nodiscard]] constexpr bool isDown(Key k) const noexcept {
    return keyStates[std::to_underlying(k)] == KeyState::Pressed ||
           keyStates[std::to_underlying(k)] == KeyState::Held;
  }

  [[nodiscard]] constexpr bool isPressed(MouseButton k) const noexcept {
    return mouseStates[std::to_underlying(k)] == KeyState::Pressed;
  }

  [[nodiscard]] constexpr bool isReleased(MouseButton k) const noexcept {
    return mouseStates[std::to_underlying(k)] == KeyState::Released;
  }

  [[nodiscard]] constexpr bool isHeld(MouseButton k) const noexcept {
    return mouseStates[std::to_underlying(k)] == KeyState::Held;
  }

  [[nodiscard]] constexpr bool isUp(MouseButton k) const noexcept {
    return mouseStates[std::to_underlying(k)] == KeyState::Idle ||
           mouseStates[std::to_underlying(k)] == KeyState::Released;
  }

  [[nodiscard]] constexpr bool isDown(MouseButton k) const noexcept {
    return mouseStates[std::to_underlying(k)] == KeyState::Pressed ||
           mouseStates[std::to_underlying(k)] == KeyState::Held;
  }

  [[nodiscard]] constexpr v2d getMouseDelta() const noexcept {
    return mousePosition - lastMousePosition;
  }

private:
  // Keyboard
  std::array<KeyState, std::to_underlying(Key::__SizeGuard)> keyStates{};

  // Mouse
  std::array<KeyState, std::to_underlying(MouseButton::__SizeGuard)>
      mouseStates{};

  v2d lastMousePosition{};
  v2d mousePosition{};
};
