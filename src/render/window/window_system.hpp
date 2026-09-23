#pragma once

class WindowSystem final {

public:
  WindowSystem() = delete;
  WindowSystem(const WindowSystem &) = delete;
  WindowSystem(WindowSystem &&) = delete;
  WindowSystem &operator=(const WindowSystem &) = delete;
  WindowSystem &operator=(WindowSystem &&) = delete;

  static void init();
  static void shutdown();

private:
  inline static bool s_isInitialized{false};
};
