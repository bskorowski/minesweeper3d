#include "scene.hpp"
#include "debug_utils.hpp"
#include "ui/scene.hpp"

void SceneManager::addScene(std::unique_ptr<Scene> scene) {
  scenes_.emplace(std::move(scene));
}

void SceneManager::popScene() {
  DEBUG_ASSERT(!scenes_.empty(),
               "To pop a scene there must be at least one scene.")
}

auto SceneManager::currentScene() -> Scene * {
  DEBUG_ASSERT(!scenes_.empty(),
               "No scene on the stack, and trying to get one.")
  return scenes_.top().get();
}

auto SceneManager::currentScene() const -> const Scene * {
  DEBUG_ASSERT(!scenes_.empty(),
               "No scene on the stack, and trying to get one.")
  return scenes_.top().get();
}
