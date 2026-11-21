#include <M5Unified.h>

#include "nexos/HomeScreen.h"
#include "nexos/SplashScreen.h"

// M5Unified reserves five button slots; the StickS3 only fills BtnA/BtnB and,
// through the PMIC, BtnPWR (power).
constexpr size_t kButtonCount = 5;

// True when any physical button is pressed this cycle except the power button.
// Pressing power must never leave the splash, so it is skipped explicitly even
// though it is reported by M5Unified.
static bool anyNonPowerButtonPressed() {
  for (size_t i = 0; i < kButtonCount; ++i) {
    if (&M5.getButton(i) == &M5.BtnPWR) {
      continue;
    }
    if (M5.getButton(i).wasPressed()) {
      return true;
    }
  }
  return false;
}

enum class Screen { kBoot, kHome };

static nexos::SplashScreen gSplash;
static nexos::HomeScreen gHome;
static Screen gScreen = Screen::kBoot;

void setup() {
  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;
  // Boot does not use audio or motion. Leaving them off avoids the codec pop on boot.
  cfg.internal_spk = false;
  cfg.internal_mic = false;
  cfg.internal_imu = false;

  M5.begin(cfg);
  gSplash.begin(M5.Display);
}

void loop() {
  M5.update();

  if (gScreen == Screen::kBoot) {
    // The home screen is not automatic: any button except power leaves the splash.
    if (anyNonPowerButtonPressed()) {
      gScreen = Screen::kHome;
      gHome.begin(M5.Display);
      Serial.println("NexOS: entered home");
      return;  // skip the extra splash frame this cycle so it is not drawn over home
    }
    gSplash.update();
  } else {
    gHome.update();
  }
}
