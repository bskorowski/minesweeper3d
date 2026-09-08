#include "scene.hpp"
#include "debug_utils.hpp"
#include "ui/scene.hpp"

void SceneManager::prepareFrame() {

  navigatedBack_ = false;
  DEBUG_ASSERT(!navigatedTo_, "navigatedTo_ has not been consumed, and it is "
                              "still present in prepareFrame()");
  // Not needed as it should be moved from in endFrame, but prefer doing it
  // explicitly
  navigatedTo_ = nullptr;

  DEBUG_ASSERT(!scenes_.empty(),
               "No scene on the stack, and trying to get one.")
  activeScene_ = scenes_.top().get();
}

void SceneManager::endFrame() {
  activeScene_ = nullptr;

  if (navigatedBack_) {
    DEBUG_ASSERT(
        scenes_.size() > 1 && !navigatedTo_,
        "Couldn't navigate back to null scene, and no replacing scene was set");
    scenes_.pop();
  }
  if (navigatedTo_) {
    scenes_.emplace(std::move(navigatedTo_));
    navigatedTo_ = nullptr; // To be explicit
  }
}

void SceneManager::navigateTo(std::unique_ptr<Scene> scene) {
  scenes_.emplace(std::move(scene));
}

void SceneManager::navigateBack() {
  DEBUG_ASSERT(scenes_.size() > 1,
               "Cannot navigate back, when no scene will be active");
  navigatedBack_ = true;
}

void SceneManager::popScene() {
  DEBUG_ASSERT(!scenes_.empty(),
               "To pop a scene there must be at least one scene.")
}

auto SceneManager::currentScene() -> Scene * {
  DEBUG_ASSERT(
      activeScene_,
      "No active scene, perhaps no SceneManager::prepareFrame() was called. ");
  return activeScene_;
}

auto SceneManager::currentScene() const -> const Scene * {
  DEBUG_ASSERT(
      activeScene_,
      "No active scene, perhaps no SceneManager::prepareFrame() was called. ");
  return activeScene_;
}
