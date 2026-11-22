#pragma once

#include <M5GFX.h>

#include "nexos/LoadingBar.h"
#include "nexos/NeonLogo.h"
#include "nexos/StatusRow.h"
#include "nexos/Wordmark.h"

namespace nexos {

class SplashScreen {
 public:
  ~SplashScreen();

  void begin(M5GFX& display);
  void update();

 private:
  void render(uint32_t elapsedMs);

  M5GFX* _display = nullptr;
  M5Canvas* _canvas = nullptr;
  NeonLogo _logo;
  Wordmark _wordmark;
  LoadingBar _bar;
  StatusRow _status;
  uint32_t _startMs = 0;
  uint32_t _lastFrameMs = 0;
  bool _bootLogged = false;
  bool _staticFallback = false;
  // Cached battery state, refreshed by StatusRow::poll() at ~1 Hz. -1 means
  // "unreadable" until the first successful read.
  int _batteryLevel = -1;
  bool _charging = false;
};

}  // namespace nexos
