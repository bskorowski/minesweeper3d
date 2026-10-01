#include "window.hpp"
#include "GLFW/glfw3.h"
#include "constants.hpp"
#include "debug.hpp"
#include "error.hpp"

Window::Window(WindowParams params)
    : title_{std::move(params.title)}, size_{params.size} {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, constants::OPENGL_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, constants::OPENGL_VERSION_MINOR);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  handle_ =
      glfwCreateWindow(cast<int>(params.size.x()), cast<int>(params.size.y()),
                       params.title.c_str(), nullptr, nullptr);

  if (handle_ == nullptr) {
    throw ERR(
        WindowError,
        std::format("Couldn't create GLFW window with name: '{}'", title_));
  }

  // Callbacks
  ASSERT(glfwSetKeyCallback(handle_, nullptr) == nullptr,
         "Key callback already set for this window, reusing a handle?");

  // TODO :: I fell like making context and enabling vsync shouldn't be here
  glfwMakeContextCurrent(handle_);

  if (params.enableVsync) {
    glfwSwapInterval(1);
  }
}

Window::~Window() { glfwDestroyWindow(handle_); }

void Window::setCursorCaptured(const bool captured) {
  ASSERT(handle_, "Trying to use unitinialized window");

  const int cursorMode = captured ? GLFW_CURSOR_CAPTURED : GLFW_CURSOR_DISABLED;

  glfwSetInputMode(handle_, GLFW_CURSOR, cursorMode);
}

auto Window::getTitle() const noexcept -> const std::string & {
  ASSERT(!title_.empty(), "Window doesn't have a name, or name is empty?");
  return title_;
}

auto Window::getSize() const noexcept -> const v2u {
  ASSERT(size_.x() > 0 && size_.y() > 0,
         "Window size should be non-zero and is: {}", size_);
  return size_;
}

auto Window::getHandle() const noexcept -> Handle {
  ASSERT(handle_ != nullptr,
         "Window handle must be non null. Perhaps not initialized?");
  return handle_;
}
