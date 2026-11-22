#include <M5Unified.h>

#include "nexos/HomeScreen.h"
#include "nexos/NavigationScreen.h"
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

enum class Screen { kBoot, kHome, kNavigation };

static nexos::SplashScreen gSplash;
static nexos::HomeScreen gHome;
static nexos::NavigationScreen gNavigation;
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
  } else if (gScreen == Screen::kHome) {
    // Idle home is a plain black screen; button A opens the navigation shell.
    gHome.update();
    if (M5.BtnA.wasPressed()) {
      gScreen = Screen::kNavigation;
      gNavigation.begin(M5.Display);
      Serial.println("NexOS: entered navigation");
      return;  // skip the extra black frame this cycle so it is not drawn over nav
    }
  } else {  // kNavigation
    gNavigation.update();
    // Button B returns to the idle home screen.
    if (M5.BtnB.wasPressed()) {
      gScreen = Screen::kHome;
      Serial.println("NexOS: returned to home");
      return;  // skip the extra nav frame this cycle so it is not drawn over home
    }
    // Button A moves the highlight to the next placeholder slot.
    if (M5.BtnA.wasPressed()) {
      gNavigation.advance();
    }
  }
}
