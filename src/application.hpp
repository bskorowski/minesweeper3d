#pragma once
#include "input.hpp"
#include "settings.hpp"
#include "ui/scene.hpp"

struct Application {

  auto initialize() -> bool;
  void run();
  auto shutdown() -> bool;

  [[nodiscard]] static auto getDeltaTime() -> float;
  [[nodiscard]] static auto getWindow() -> GLFWwindow *;
  [[nodiscard]] static auto getInput() -> Input &;
  [[nodiscard]] static auto getSceneManager() -> SceneManager &;
  [[nodiscard]] static auto getSettings() -> Settings &;

private:
  // These fields can be safely assumed to be always available after app
  // initialization.
  // They can be safely globally accessed.

  inline static float deltaTime_{0.0f};
  // TODO :: Refactor this out into our Window wrapper class
  inline static GLFWwindow *mainWindow_{nullptr};
  inline static Input input_{};
  inline static SceneManager sceneManager_{};
  inline static Settings settings_{};
};
