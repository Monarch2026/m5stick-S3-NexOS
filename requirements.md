# Requirements

## Current

NexOS boots on an M5Stack StickS3 with an animated splash, then offers an empty home screen.

### Splash (boot)

- The screen is black, high contrast, and neon, in the spirit of an Apple boot splash: one centered mark, the product name, and a loading bar.
- The mark is a diamond with four spikes, one leaving each vertex (up, right, down, and left).
- The name is **NexOS**.
- The loading bar fills once, then stays full. A slow highlight keeps moving across it.
- After the intro, the mark stays on screen, the neon halo breathes, and a faint "Press any button" hint invites interaction.

### Home screen

- Pressing **any button except the power button** leaves the finished splash for the home screen. The power button never enters it.
- There are no apps yet. The home is an idle screen: the same neon mark and "NexOS" wordmark centered, with the halo gently breathing — exactly where an app grid will eventually live.

### Status row (both splash and home)

- A small status row sits in the top-right corner on both screens: a battery icon filled to the remaining charge, its percentage, and a charging indicator (a green bolt above the icon while charging).
- No clock. The StickS3 has no wall-clock source without network or an external RTC, both out of scope; there is intentionally no time display.
- The percentage comes from `M5.Power.getBatteryLevel()` (0–100 for the StickS3 PMIC) and the charge state from `M5.Power.isCharging()`. Reads are throttled to about once per second so the I2C read does not happen every frame.

## Device

- Board: M5Stack StickS3 (ESP32-S3, 8 MB flash, 8 MB PSRAM).
- Display: ST7789P3, 135 × 240, portrait, rotation 0.
- Toolchain: PlatformIO, Arduino framework, M5Unified.

## Out of scope for this version

- App launcher, extra apps, settings, sound, and network.
