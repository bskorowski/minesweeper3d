#include "profiling.hpp"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include <stdint.h>

void ProfilerData::draw() const {
  ImGui::SetNextWindowPos({0, 0});
  ImGuiWindowFlags flags = 0;
  flags |= ImGuiWindowFlags_NoTitleBar;
  flags |= ImGuiWindowFlags_NoCollapse;
  flags |= ImGuiWindowFlags_NoResize;
  flags |= ImGuiWindowFlags_NoMove;
  flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  flags |= ImGuiWindowFlags_NoNavFocus;
  flags |= ImGuiWindowFlags_AlwaysAutoResize;

  ImGui::Begin("Frame data", nullptr, flags);
  ImGui::Text("FPS: %d", static_cast<uint32_t>(1.0 / totalFrameMs * 1000.0));
  ImGui::Text("Frame time [ms]: %.3f", totalFrameMs);
  ImGui::Text("CPU update time [ms]: %.3f", updateMs);
  ImGui::Text("CPU Render time [ms]: %.3f", cpuRenderMs);
  ImGui::Text("GPU Render time [ms]: %.3f", gpuRenderMs);
  ImGui::Text("UI Update time [ms]: %.3f", uiUpdateMs);
  ImGui::Text("UI Render time [ms]: %.3f", uiRenderMs);
  ImGui::Text("Wait  time [ms]: %.3f", waitTime);
  ImGui::Text("Frame number: %llu", frameCounter);
  ImGui::End();
}

ScopedTimer::ScopedTimer(double &out) noexcept : outputMs_{out} {
  start_ = glfwGetTime();
}

ScopedTimer::~ScopedTimer() noexcept {
  // Second to ms
  outputMs_ = (glfwGetTime() - start_) * 1000.0;
}

Timer::Timer() noexcept { reset(); }

double Timer::reset() noexcept {
  double newTime = glfwGetTime();
  double secondsElapsed = (glfwGetTime() - time_);
  time_ = newTime;
  return secondsElapsed;
}
