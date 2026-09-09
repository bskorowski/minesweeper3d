#include "game_scene.hpp"
#include "GLFW/glfw3.h"
#include "application.hpp"
#include "imgui.h"
#include "logzy/logzy.hpp"
#include "math/matrix.hpp"
#include "resource_manager.hpp"
#include "ui_utils.hpp"

constexpr v2u crosshairSize = vec2(10u, 10u);
constexpr v3f crosshairColorNormalizedRGB = vec3(0.0f, 0.0f, 1.0f);

static void loadTextures() {
  std::array<std::string_view, 29> paths{
      "tiles/tile_0.jpg",    "tiles/tile_1.jpg",   "tiles/tile_2.jpg",
      "tiles/tile_3.jpg",    "tiles/tile_4.jpg",   "tiles/tile_5.jpg",
      "tiles/tile_6.jpg",    "tiles/tile_7.jpg",   "tiles/tile_8.jpg",
      "tiles/tile_9.jpg",    "tiles/tile_10.jpg",  "tiles/tile_11.jpg",
      "tiles/tile_12.jpg",   "tiles/tile_13.jpg",  "tiles/tile_14.jpg",
      "tiles/tile_15.jpg",   "tiles/tile_16.jpg",  "tiles/tile_17.jpg",
      "tiles/tile_18.jpg",   "tiles/tile_19.jpg",  "tiles/tile_20.jpg",
      "tiles/tile_21.jpg",   "tiles/tile_22.jpg",  "tiles/tile_23.jpg",
      "tiles/tile_24.jpg",   "tiles/tile_25.jpg",  "tiles/tile_26.jpg",
      "tiles/tile_flag.jpg", "tiles/tile_bomb.jpg"};

  if (ResourceManager::loadTextureArray(ResourceManager::TileTexturesKey,
                                        std::span{paths})) {
    logzy::info("Loaded texture array: {}", ResourceManager::TileTexturesKey);
  } else {
    logzy::critical("Failed to load texture: {}",
                    ResourceManager::TileTexturesKey);
  }
}

GameScene::GameScene()
    : crosshair_{getWindowSize(), crosshairSize, crosshairColorNormalizedRGB} {
  constexpr size_t BOARD_SIZE{10};
  if (auto boardOpt = Board::create(v3uz{BOARD_SIZE, BOARD_SIZE, BOARD_SIZE})) {
    logzy::info("board created with size: {}", BOARD_SIZE);
    board_ = std::move(boardOpt).value();
  } else {
    logzy::critical("Couldn't create board with size: {}", BOARD_SIZE);
    throw std::runtime_error(std::format("Couldnt' generate board"));
  }
  glfwSetInputMode(Application::getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  loadTextures();
}

void GameScene::handleInputs() {
  const Input &input = Application::getInput();
  GLFWwindow *window = Application::getWindow();

  // Showing cursor when alt is pressed
  if (input.isPressed(Key::Escape)) {
    pauseMenuOpen_ = !pauseMenuOpen_;
    // TODO :: Refactor this out into our Window wrapper class
    if (pauseMenuOpen_) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
  }

  if (input.isPressed(Key::F1)) {
    profilerMenuOpen_ = !profilerMenuOpen_;
  }

  // Game inputs are ignored in cursor/ui mode
  if (pauseMenuOpen_) {
    return;
  }

  cameraLookDelta_ = castAs<float>(input.getMouseDelta());
  cameraMoveDelta_ = vec3(0.0f, 0.0f, 0.0f);

  if (input.isDown(Key::A)) {
    cameraMoveDelta_.x() -= 1.0f;
  }
  if (input.isDown(Key::D)) {
    cameraMoveDelta_.x() += 1.0f;
  }
  if (input.isDown(Key::W)) {
    cameraMoveDelta_.z() -= 1.0f;
  }
  if (input.isDown(Key::S)) {
    cameraMoveDelta_.z() += 1.0f;
  }
  if (input.isDown(Key::Space)) {
    cameraMoveDelta_.y() += 1.0f;
  }
  if (input.isDown(Key::LeftControl)) {
    cameraMoveDelta_.y() -= 1.0f;
  }

  if (input.isPressed(Key::N)) {
    board_.toggleDrawNeighbours(!board_.drawDugAdjacent);
  }

  if (input.isPressed(MouseButton::Left)) {
    board_.onLeftClick(camera_.position, camera_.getDirection());
  }

  if (input.isPressed(MouseButton::Right)) {
    board_.onRightClick(camera_.position, camera_.getDirection());
  }
}

void GameScene::update() {
  const Settings &settings = Application::getSettings();
  const float dt = Application::getDeltaTime();

  // Camera lookaround movement
  const float cameraXDelta = -cameraLookDelta_.x() * settings.sensitivity * dt;
  const float cameraYDelta = cameraLookDelta_.y() * settings.sensitivity * dt;
  camera_.rotate(vec3<float>(cameraYDelta, cameraXDelta, 0.0F));

  // Camera position movement
  const float cameraTravelledDistance = settings.cameraSpeed * dt;
  camera_.move(cameraMoveDelta_, cameraTravelledDistance);
};

void GameScene::draw() {
  v2u windowSize = getWindowSize();

  constexpr float fov = 50.0f;
  constexpr float near = 0.01f;
  constexpr float far = 100.0f;
  const float ratio = static_cast<float>(windowSize.x()) / windowSize.y();
  auto persp = perspective(fov, ratio, near, far);
  DEBUG_ASSERT(ratio > 0.0f, "Ratio must be positive");

  board_.draw(camera_.getView(), persp);
};

static void drawPauseMenu();

void GameScene::updateAndDrawUI() {
  v2u windowSize = getWindowSize();

  auto ortho = orthographic(0.0f, windowSize.x(), 0.0F, windowSize.y(), -1.0f);
  crosshair_.draw(ortho);

  if (profilerMenuOpen_) {
    // TODO :: Render profiler data
    // drawRenderData(profilerData_);
  }

  if (pauseMenuOpen_) {
    drawPauseMenu();
  }
};

static void drawPauseMenu() {

  GLFWwindow *window = Application::getWindow();
  Settings &settings = Application::getSettings();

  constexpr float minMovementSpeed = 1.0f;
  constexpr float maxMovementSpeed = 20.0f;

  constexpr float minSensitivity = 0.01f;
  constexpr float maxSensitivity = 20.0f;

  {
    const ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoCollapse;
    flags |= ImGuiWindowFlags_NoResize;
    flags |= ImGuiWindowFlags_NoMove;
    ImGui::Begin("Settings", nullptr, flags);

    ImGui::SetWindowFontScale(3.0f);
    ImVec2 cursorBeforeMenu = ImGui::GetCursorPos();

    const char *menuText = "Menu";
    ImVec2 menuTextSize = ImGui::CalcTextSize(menuText);
    float menuX = (vp->WorkSize.x - menuTextSize.x) * 0.5f;
    ImGui::SetCursorPos((ImVec2(menuX, cursorBeforeMenu.y)));
    ImGui::Text("%s", menuText);
    ImGui::SetCursorPos(
        (ImVec2(cursorBeforeMenu.x, cursorBeforeMenu.y + menuTextSize.y)));
    ImGui::SetWindowFontScale(1.0f);

    ImGui::Separator();

    {
      ImGui::SetWindowFontScale(2.0f);
      ImGui::Text("%s", "Settings");
      ImGui::SetWindowFontScale(1.0f);
      constexpr float sliderMaxWidth = 400.0f;
      float availableWidth = ImGui::GetContentRegionAvail().x;
      float sliderWidth = std::min(availableWidth, sliderMaxWidth);
      ImGui::PushItemWidth(sliderWidth);
      ImGui::SliderFloat("Mouse sensitivity", &settings.sensitivity,
                         minSensitivity, maxSensitivity);
      ImGui::SliderFloat("Player movement speed", &settings.cameraSpeed,
                         minMovementSpeed, maxMovementSpeed);
      ImGui::PopItemWidth();
    }
    ImGui::Separator();

    ImGui::SetWindowFontScale(2.0f);
    ImGui::Text("Controls");
    ImGui::SetWindowFontScale(1.0f);

    static constexpr std::array controls{
        std::pair{"Escape", "Toggle Menu / Release Mouse"},
        std::pair{"F1", "Toggle Debug/Profiler Window"},
        std::pair{"W, A, S, D", "Movement (Forward, Left, Back, Right)"},
        std::pair{"Space", "Move Up"},
        std::pair{"Left Ctrl", "Move Down"},
        std::pair{"Mouse", "Look Around"},
        std::pair{"Left Click", "Dig / Reveal Cell"},
        std::pair{"Right Click", "Place Flag"},
        std::pair{"N", "Toggle dug adjacent cell visibility"}};

    for (const auto &[key, description] : controls) {
      ImGui::Text("%s - %s", key, description);
    }

    if (ImGui::Button("Back to menu")) {
      Application::getSceneManager().navigateBack();
    }
    ImGui::End();
  }
}
