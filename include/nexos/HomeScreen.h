#pragma once

#include <M5GFX.h>

#include "nexos/NeonLogo.h"
#include "nexos/Wordmark.h"

namespace nexos {

// Idle home screen. No apps yet: it shows the neon mark and wordmark with a
// slowly breathing halo, exactly where an app grid will eventually live.
class HomeScreen {
 public:
  ~HomeScreen();

  void begin(M5GFX& display);
  void update();

 private:
  void render(uint32_t elapsedMs);

  M5GFX* _display = nullptr;
  M5Canvas* _canvas = nullptr;
  NeonLogo _logo;
  Wordmark _wordmark;
  uint32_t _startMs = 0;
  uint32_t _lastFrameMs = 0;
};

}  // namespace nexos
