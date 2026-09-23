#pragma once
#include "input.hpp"
#include "profiling.hpp"
#include "render/window/window.hpp"
#include "settings.hpp"
#include "ui/scene.hpp"

struct Application {

  auto initialize() -> bool;
  void run();
  auto shutdown() -> bool;

  [[nodiscard]] static auto getDeltaTime() noexcept -> double;
  [[nodiscard]] static auto getWindow() noexcept -> Window &;
  [[nodiscard]] static auto getInput() noexcept -> const Input &;
  [[nodiscard]] static auto getProfilerData() noexcept -> const ProfilerData &;
  [[nodiscard]] static auto getSceneManager() noexcept -> SceneManager &;
  [[nodiscard]] static auto getSettings() noexcept -> Settings &;

private:
  // These fields can be safely assumed to be always available after app
  // initialization.
  // They can be safely globally accessed.

  inline static double deltaTime_{0.0f};
  inline static std::unique_ptr<Window> mainWindow_{nullptr};
  inline static Input input_{};
  inline static SceneManager sceneManager_{};
  inline static Settings settings_{};
  inline static ProfilerData profilerData_{};
};
