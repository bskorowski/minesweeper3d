#pragma once

#include <stdint.h>

struct ProfilerData {
  double totalFrameMs{0.0};
  double updateMs{0.0};
  double cpuRenderMs{0.0};
  double gpuRenderMs{0.0};
  double uiUpdateMs{0.0};
  double uiRenderMs{0.0};
  double waitTime{0.0};
  uint64_t frameCounter{0};

  void draw() const;
};

class ScopedTimer {

public:
  ScopedTimer(double &out) noexcept;

  ~ScopedTimer() noexcept;

private:
  double start_;
  double &outputMs_;
};

class Timer {

public:
  Timer() noexcept;
  // Resets the clock and returns elapsed time
  double reset() noexcept;

private:
  double time_;
};
