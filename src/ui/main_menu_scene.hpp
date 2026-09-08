#pragma once

#include "scene.hpp"

class MainMenuScene final : public Scene {

public:
  virtual ~MainMenuScene() = default;
  void updateAndDrawUI() override;
};
