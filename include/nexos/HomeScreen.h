#pragma once

#include <M5GFX.h>

#include "nexos/NeonLogo.h"
#include "nexos/StatusRow.h"
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
  StatusRow _status;
  uint32_t _startMs = 0;
  uint32_t _lastFrameMs = 0;
  // Cached battery state, refreshed by StatusRow::poll() at ~1 Hz. -1 means
  // "unreadable" until the first successful read.
  int _batteryLevel = -1;
  bool _charging = false;
};

}  // namespace nexos
