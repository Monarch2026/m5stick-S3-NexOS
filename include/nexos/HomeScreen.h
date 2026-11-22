#pragma once

#include <M5GFX.h>

namespace nexos {

// Idle home screen: a plain black background. The neon mark, wordmark, and
// status row were removed on purpose — this is the resting state before the
// navigation shell is opened with button A. Kept as its own screen so boot
// stays a clean state machine and the sprite pattern stays reusable.
class HomeScreen {
 public:
  ~HomeScreen();

  void begin(M5GFX& display);
  void update();

 private:
  void render(uint32_t elapsedMs);

  M5GFX* _display = nullptr;
  M5Canvas* _canvas = nullptr;
  uint32_t _startMs = 0;
  uint32_t _lastFrameMs = 0;
};

}  // namespace nexos
