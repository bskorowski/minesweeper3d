#include "application.hpp"

#include "debug_utils.hpp"
#include "glad.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "profiling.hpp"
#include "resource_manager.hpp"
#include "settings.hpp"
#include "ui/main_menu_scene.hpp"
#include "ui/scene.hpp"
#include <GLFW/glfw3.h>
#include <logzy/logzy.hpp>

static void GLFWErrorCallback(int code, const char *description) {
  logzy::error("GLFW Error occurred. Code {}. Description: {}", code,
               description);
}

static auto initializeGLFW() -> bool {
  if (!glfwInit()) {
    logzy::critical("GLFW could not be initialized. glfwInit() failed.");
    return false;
  }
  return true;
}

/**
 * Creates  GLFW window with OPENGL 4.6 core as render context.
 */
static auto createGLFWWindow() -> GLFWwindow * {
  // Why not use the newest one I guess
  constexpr int OPENGL_VERSION_MAJOR = 4;
  constexpr int OPENGL_VERSION_MINOR = 6;

  // Not using deprecated functions, no compatiblilty with older opengl
  constexpr int OPENGL_PROFILE = GLFW_OPENGL_CORE_PROFILE;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
  glfwWindowHint(GLFW_OPENGL_PROFILE, OPENGL_PROFILE);

  constexpr std::uint32_t INITIAL_WINDOW_WIDTH = 800;
  constexpr std::uint32_t INITIAL_WINDOW_HEIGHT = 800;
  constexpr const char *MAIN_WINDOW_TITLE = "Minesweeper 3D";

  GLFWwindow *window =
      glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT,
                       MAIN_WINDOW_TITLE, nullptr, nullptr);

  return window;
}

static auto initializeMainGLFWWindow(GLFWwindow *window) -> bool {
  // Callbacks
  DEBUG_ASSERT(glfwSetErrorCallback(nullptr) == nullptr,
               "Making sure no duplicate error callback is set");
  DEBUG_ASSERT(glfwSetKeyCallback(window, nullptr) == nullptr,
               "Making sure no duplicate key callback is set");

  // TODO :: In theory these could fail too
  glfwSetErrorCallback(GLFWErrorCallback);

  // Disabling cursor when focused
  return true;
}

static void intializeOpenGL(GLFWwindow *window) {
  // TODO :: In theory these could fail too
  // OpenGL stuff
  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);

  // VSYNC ON
  glfwSwapInterval(1);

  // OpenGl global state
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  // for transparnets
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

static void initializeDearImgui(GLFWwindow *window) {
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
  ImGui_ImplGlfw_InitForOpenGL(window, installCallbacks);
  ImGui_ImplOpenGL3_Init("#version 330 core");
}

auto Application::initialize() -> bool {
  if (!initializeGLFW()) {
    logzy::critical("GLFW could not be initialized");
    return false;
  }

  mainWindow_ = createGLFWWindow();

  if (mainWindow_ == nullptr) {
    logzy::critical("Couldn't create the main window of the application.");
    return false;
  }

  if (!initializeMainGLFWWindow(mainWindow_)) {
    logzy::critical("Initialization of main window failed.");
    return false;
  }

  intializeOpenGL(mainWindow_);
  initializeDearImgui(mainWindow_);

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
  while (!glfwWindowShouldClose(mainWindow_)) {
    sceneManager_.prepareFrame();
    deltaTime_ = deltaTimer.reset();
    Scene *currentScene = sceneManager_.currentScene();
    glfwPollEvents();
    input_.update(mainWindow_);

    double time = static_cast<float>(glfwGetTime());
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
      glfwSwapBuffers(mainWindow_);
      frameSync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    }

    sceneManager_.endFrame();
  }
  glDeleteQueries(2, queryID);
}

auto Application::shutdown() -> bool {
  if (mainWindow_ == nullptr) {
    logzy::error("Trying to shutdown application that wasn't initialized with "
                 "Application::initialize()");
    return false;
  }
  glfwDestroyWindow(mainWindow_);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  // Shutting odwn glfw
  glfwDestroyWindow(mainWindow_);
  mainWindow_ = nullptr;
  glfwTerminate();

  return true;
}

auto Application::getDeltaTime() noexcept -> double { return deltaTime_; }

auto Application::getWindow() noexcept -> GLFWwindow * { return mainWindow_; }

auto Application::getInput() noexcept -> const Input & { return input_; }

auto Application::getProfilerData() noexcept -> const ProfilerData & {
  return profilerData_;
}

auto Application::getSceneManager() noexcept -> SceneManager & {
  return sceneManager_;
}

auto Application::getSettings() noexcept -> Settings & { return settings_; }
