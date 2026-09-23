#include "window_system.hpp"
#include "GLFW/glfw3.h"
#include "debug.hpp"
#include "error.hpp"
#include "logzy/logzy.hpp"

namespace {
void GLFWErrorCallback(int code, const char *description) {
  logzy::error("GLFW Error occurred. Code {}. Description: {}", code,
               description);
}
} // namespace

void WindowSystem::init() {
  ASSERT(!s_isInitialized,
         "Window system shouldn't be initialized more than once");
  if (!glfwInit()) {
    logzy::critical("Couldn't initialize GLFW");
    throw ERR(WindowError, "Couldn't initialize GLFW");
  }

  ASSERT(glfwSetErrorCallback(nullptr) == nullptr,
         "Making sure no duplicate error callback is set");
  glfwSetErrorCallback(GLFWErrorCallback);

  s_isInitialized = true;
}

void WindowSystem::shutdown() {
  s_isInitialized = false;
  glfwTerminate();
}
