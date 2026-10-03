#include "main_menu_scene.hpp"
#include "application.hpp"
#include "components/components.hpp"
#include "imgui.h"
#include "ui/components/modifier.hpp"
#include "ui/game_scene.hpp"

void MainMenuScene::updateAndDrawUI() {
  SceneManager &sceneManager = Application::getSceneManager();
  const ImGuiViewport *vp = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(vp->WorkPos);
  ImGui::SetNextWindowSize(vp->WorkSize);
  ImGuiWindowFlags flags = 0;
  flags |= ImGuiWindowFlags_NoTitleBar;
  flags |= ImGuiWindowFlags_NoCollapse;
  flags |= ImGuiWindowFlags_NoResize;
  flags |= ImGuiWindowFlags_NoMove;

  ImGui::Begin("Settings", nullptr, flags);
  components::Text("Minesweeper 3D", TextStyle{}.fontSize(24.f),
                   Modifier{}.fillWidth().centerHorizontally());

  {
    const char *buttonText = "Play";
    ImVec2 cursorBeforeButton = ImGui::GetCursorPos();
    ImGui::SetWindowFontScale(3.0f);
    ImGui::PushItemWidth(200.f);

    ImVec2 buttonTextsize = ImGui::CalcTextSize(buttonText);
    float buttonX = (vp->WorkSize.x - buttonTextsize.x) * 0.5f;
    ImGui::SetCursorPos((ImVec2(buttonX, cursorBeforeButton.y + 40.0f)));
    if (ImGui::Button(buttonText)) {
      sceneManager.navigateTo(std::make_unique<GameScene>());
    }
    ImGui::SetCursorPos(cursorBeforeButton);
  }

  ImGui::End();
}
