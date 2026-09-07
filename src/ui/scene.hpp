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
  void addScene(std::unique_ptr<Scene> scene);
  void popScene();
  [[nodiscard]] auto currentScene() -> Scene *;
  [[nodiscard]] auto currentScene() const -> const Scene *;

private:
  std::stack<std::unique_ptr<Scene>> scenes_;
};
