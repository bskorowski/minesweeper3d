#include "ui_utils.hpp"
#include "application.hpp"
#include "debug.hpp"

v2u getWindowSize() {
  // TODO :: Also this when refactorin window wrapper
  v2i dimensions;
  glfwGetWindowSize(Application::getWindow(), &dimensions.x(), &dimensions.y());
  ASSERT(dimensions.x() > 0 && dimensions.y() > 0,
               "Failed to fetch window dimensions");
  return castAs<uint32_t>(dimensions);
}


