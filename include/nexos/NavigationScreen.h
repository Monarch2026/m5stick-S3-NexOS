#pragma once

#include <M5GFX.h>

#include "nexos/NavigationGrid.h"
#include "nexos/StatusRow.h"

namespace nexos {

// Launcher navigation shell: an empty neon app grid on a black background.
// Reached from the idle home screen; there are no apps yet, so it only shows
// placeholder slots and a breathing highlight you move between. Same screen
// shape as the splash and home — one object owns timing and draws into a PSRAM
// sprite pushed to the panel each frame.
class NavigationScreen {
 public:
  ~NavigationScreen();

  void begin(M5GFX& display);
  void update();

  // Move the highlight to the next slot (wraps). No-op if the sprite failed.
  void advance();

 private:
  void render(uint32_t elapsedMs);

  M5GFX* _display = nullptr;
  M5Canvas* _canvas = nullptr;
  NavigationGrid _grid;
  StatusRow _status;
  uint32_t _startMs = 0;
  uint32_t _lastFrameMs = 0;
  // Cached battery state, refreshed by StatusRow::poll() at ~1 Hz. -1 means
  // "unreadable" until the first successful read.
  int _batteryLevel = -1;
  bool _charging = false;
};

}  // namespace nexos
