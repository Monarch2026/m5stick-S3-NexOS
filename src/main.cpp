#include <M5Unified.h>

#include "nexos/SplashScreen.h"

nexos::SplashScreen gSplash;

void setup() {
  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;
  // Splash does not use audio or motion. Leaving them off avoids the codec pop on boot.
  cfg.internal_spk = false;
  cfg.internal_mic = false;
  cfg.internal_imu = false;

  M5.begin(cfg);
  gSplash.begin(M5.Display);
}

void loop() {
  M5.update();
  gSplash.update();
}
