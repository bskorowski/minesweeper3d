#include "main_menu_scene.hpp"
#include "application.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
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
  {
    ImVec2 cursorBeforeMenu = ImGui::GetCursorPos();
    ImGui::SetWindowFontScale(7.0f);
    const char *menuText = "Minesweeper 3D";
    ImVec2 menuTextSize = ImGui::CalcTextSize(menuText);
    float menuX = (vp->WorkSize.x - menuTextSize.x) * 0.5f;
    ImGui::SetCursorPos((ImVec2(menuX, cursorBeforeMenu.y)));
    ImGui::Text("%s", menuText);
    ImGui::SetCursorPos(
        (ImVec2(cursorBeforeMenu.x, cursorBeforeMenu.y + menuTextSize.y)));
    ImGui::SetWindowFontScale(1.0f);
  }

  {
    const char *buttonText = "Play";
    ImVec2 cursorBeforeButton = ImGui::GetCursorPos();
    ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(cursor + ImVec2(0, 40.f));
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
