#pragma once
#include "input.hpp"
#include <memory>
#include <stack>

class Scene {
public:
  virtual ~Scene() = default;
  virtual void handleInputs() {};
  virtual void update() {};
  virtual void draw() {};
  virtual void updateAndDrawUI() {};
};

class SceneManager {

public:
  // Prepares the state for the frame
  void prepareFrame();
  // Cleans up state of the frame
  void endFrame();
  // Navigate to the scene on next frame
  void navigateTo(std::unique_ptr<Scene> scene);
  void navigateBack();
  void popScene();
  [[nodiscard]] auto currentScene() -> Scene *;
  [[nodiscard]] auto currentScene() const -> const Scene *;

private:
  // Scene active in this frame.
  // Should be set via prepareFrame() at the beginning of each frame
  Scene *activeScene_{nullptr};
  std::stack<std::unique_ptr<Scene>> scenes_;

  // Whether should pop the current top scene from the stack on end of frame
  bool navigatedBack_{false};
  // Whether should navigate to another scene (add to stack) on end of frame
  std::unique_ptr<Scene> navigatedTo_{nullptr};
};
