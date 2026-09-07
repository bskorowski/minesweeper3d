#pragma once

#include "game/board.hpp"
#include "game/crosshair.hpp"
#include "render/camera.hpp"
#include "scene.hpp"

class GameScene final : public Scene {

public:
  GameScene();
  ~GameScene() = default;
  void handleInputs() override;
  void update() override;
  void draw() override;
  void updateAndDrawUI() override;

private:
  // Constants
  static constexpr v3f CAMERA_INITIAL_POS{vec3(0.0f, 0.0f, 20.0f)};
  static constexpr v3f CAMERA_ARBITRARY_UP{vec3(0.0f, 1.0f, 0.0f)};

  // UI state
  bool pauseMenuOpen_{false};
  bool profilerMenuOpen_{false};
  Crosshair crosshair_;

  // Render state
  // Difference of the mouse delta, effectively if the mouse moved
  v2f cameraLookDelta_{};
  // Direction where the camera moved
  v3f cameraMoveDelta_{};
  Camera camera_{CAMERA_INITIAL_POS, CAMERA_ARBITRARY_UP};

  // Game state
  Board board_{};
};
