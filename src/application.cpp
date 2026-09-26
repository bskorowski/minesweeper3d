#include "application.hpp"

#include "constants.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "glad.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "profiling.hpp"
#include "render/window/window.hpp"
#include "render/window/window_system.hpp"
#include "resource_manager.hpp"
#include "settings.hpp"
#include "ui/main_menu_scene.hpp"
#include "ui/scene.hpp"
#include <GLFW/glfw3.h>
#include <logzy/logzy.hpp>
#include <memory>

static void intializeOpenGL() {
  logzy::debug("Initializing OpenGL");
  // OpenGL stuff
  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    throw ERR(GraphicsError, "gladLoadGL failed");
  }

  // OpenGl global state
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  // for transparnets
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

static void initializeDearImgui(const Window &window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();

  float scale =
      ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

  ImGuiStyle &style = ImGui::GetStyle();
  style.ScaleAllSizes(scale);
  style.FontScaleDpi = scale;

  bool installCallbacks = true;
  ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), installCallbacks);
  ImGui_ImplOpenGL3_Init(constants::OPENLG_GLSL_VERSION);
}

auto Application::initialize() -> bool {
  WindowSystem::init();

  mainWindow_ = std::make_unique<Window>(WindowParams{});

  intializeOpenGL();
  ASSERT(mainWindow_, "Window should be initalized here");
  initializeDearImgui(getWindow());

  sceneManager_.navigateTo(std::make_unique<MainMenuScene>());
  ResourceManager::loadFont(ResourceManager::ResourceKey::FontRegular,
                            "fonts/michroma/Michroma-Regular.ttf");

  ImGui::PushFont(static_cast<ImFont *>(
      ResourceManager::getFont(ResourceManager::ResourceKey::FontRegular)
          .fontData));

  return true;
}

void Application::run() {
  // triple buffering
  constexpr int queryBuffers = 3;
  GLuint queryID[queryBuffers];
  glGenQueries(queryBuffers, queryID);
  printf("queryID[0]=%u queryID[1]=%u\n", queryID[0], queryID[1]);
  static GLsync frameSync = nullptr;

  Timer deltaTimer{};
  while (!glfwWindowShouldClose(mainWindow_->getHandle())) {
    sceneManager_.prepareFrame();
    deltaTime_ = deltaTimer.reset();
    Scene *currentScene = sceneManager_.currentScene();
    glfwPollEvents();
    input_.update(getWindow());

    ++profilerData_.frameCounter;
    {
      ScopedTimer waitTimer(profilerData_.waitTime);
      if (frameSync) {
        glClientWaitSync(frameSync, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000);
        glDeleteSync(frameSync);
        frameSync = nullptr;
      }
    }

    // Seconds to ms
    profilerData_.totalFrameMs = Application::getDeltaTime() * 1000.0;
    {
      ScopedTimer updateTimer(profilerData_.updateMs);
      currentScene->handleInputs();
      currentScene->update();
    }

    {
      ScopedTimer renderTimer(profilerData_.cpuRenderMs);
      // Writing
      const int frontBuffer = profilerData_.frameCounter % queryBuffers;
      // Reading buffer delayed by queryBuffers-1 frames
      const int backBuffer =
          (profilerData_.frameCounter - (queryBuffers - 1) + queryBuffers) %
          queryBuffers;

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glBeginQuery(GL_TIME_ELAPSED, queryID[frontBuffer]);

      currentScene->draw();

      glEndQuery(GL_TIME_ELAPSED);
      GLuint available = 1;
      glGetQueryObjectuiv(queryID[backBuffer], GL_QUERY_RESULT_AVAILABLE,
                          &available);

      if (profilerData_.frameCounter >= 3 && available) {
        GLuint64 nanosElapsed = 0;
        glGetQueryObjectui64v(queryID[backBuffer], GL_QUERY_RESULT,
                              &nanosElapsed);
        profilerData_.gpuRenderMs = nanosElapsed / 1'000'000.0;
      }
    }
    {
      ScopedTimer uiTimer(profilerData_.uiUpdateMs);
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      currentScene->updateAndDrawUI();
      ImGui::Render();
    }
    {
      ScopedTimer uiTimer(profilerData_.uiRenderMs);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    {
      ScopedTimer systemTimer(profilerData_.waitTime);
      glfwSwapBuffers(mainWindow_->getHandle());
      frameSync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    }

    sceneManager_.endFrame();
  }
  glDeleteQueries(2, queryID);
}

auto Application::shutdown() -> bool {
  ASSERT(mainWindow_,
         "Shutting down application without a window. Not initialized?");

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  // Shutting odwn glfw
  mainWindow_.reset();
  WindowSystem::shutdown();

  return true;
}

auto Application::getDeltaTime() noexcept -> double { return deltaTime_; }

auto Application::getWindow() noexcept -> Window & {
  ASSERT(mainWindow_, "Trying to get main window which is not set. Technically "
                      "shouldn't happen");
  return *mainWindow_;
}

auto Application::getInput() noexcept -> const Input & { return input_; }

auto Application::getProfilerData() noexcept -> const ProfilerData & {
  return profilerData_;
}

auto Application::getSceneManager() noexcept -> SceneManager & {
  return sceneManager_;
}

auto Application::getSettings() noexcept -> Settings & { return settings_; }
