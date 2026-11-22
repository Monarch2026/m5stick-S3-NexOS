#pragma once

#include <M5GFX.h>

namespace nexos {

// Battery status drawer: a filled battery icon, its percentage, and a charging
// indicator (a small bolt above the icon). Pure draw-only with no state.
//
// Call poll() to refresh the cached values from M5.Power; it is throttled to
// roughly 1 Hz so the I2C read does not happen every frame. The cache lives in
// the caller: poll() only writes through the pointers when it actually reads,
// so a throttled call leaves them holding the last known values.
class StatusRow {
 public:
  // Draw in the top-right corner. level is 0..100; pass < 0 for "unreadable".
  void draw(LovyanGFX* gfx, int level, bool charging) const;

  // Refresh *level and *charging from M5.Power, throttled to ~1 Hz.
  static void poll(int* level, bool* charging);

 private:
  // Battery I2C reads are throttled to about once per second.
  static constexpr uint32_t kPollIntervalMs = 1000;
  static uint32_t sLastPollMs;
};

}  // namespace nexos
