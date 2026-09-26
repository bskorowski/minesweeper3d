#pragma once

#include "math/matrix.hpp"

struct GLFWwindow;

struct WindowParams {
  std::string title = "Minesweeper 3D";
  v2u size = vec2(800u, 800u);
  bool enableVsync = true;
};

struct Window {

  using Handle = GLFWwindow *;

public:
  Window(WindowParams params);
  ~Window();
  Window(const Window &) = delete;
  Window(Window &&) = delete;
  Window &operator=(const Window &) = delete;
  Window &operator=(Window &&) = delete;

public:
  void setCursorCaptured(bool captured);
  [[nodiscard]] auto getTitle() const noexcept -> const std::string &;
  [[nodiscard]] auto getSize() const noexcept -> const v2u;
  [[nodiscard]] auto getHandle() const noexcept -> Handle;

private:
  std::string title_{};
  Handle handle_{nullptr};
  v2u size_ = vec2(0u, 0u);
};
