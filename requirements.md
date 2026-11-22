# Requirements

## Current

NexOS boots on an M5Stack StickS3 with an animated splash, then a plain black idle home and an empty navigation shell.

### Splash (boot)

- The screen is black, high contrast, and neon, in the spirit of an Apple boot splash: one centered mark, the product name, and a loading bar.
- The mark is a diamond with four spikes, one leaving each vertex (up, right, down, and left).
- The name is **NexOS**.
- The loading bar fills once, then stays full. A slow highlight keeps moving across it.
- After the intro, the mark stays on screen, the neon halo breathes, and a faint "Press any button" hint invites interaction.

### Home screen

- Pressing **any button except the power button** leaves the finished splash for the home screen. The power button never enters it.
- The home is a plain black idle screen: no mark, no name, nothing else. It is the resting state before the navigation shell is opened with **button A**.

### Navigation (launcher shell)

- Pressing **button A** on the black home opens the navigation shell: an empty app grid styled like the splash — neon placeholder slots on a black background with a breathing halo highlight, and the same status row in the top-right corner.
- There are no apps yet, so every slot is an empty placeholder. **Button A** moves the highlight to the next slot (it wraps around the grid); **button B** returns to the black home screen. The power button never navigates.
- Selecting a slot does nothing further because there are no apps — this shell exists only as the foundation the tiny apps pack will later fill in.

### Status row (both splash and navigation)

- A small status row sits in the top-right corner on both screens: a battery icon filled to the remaining charge, its percentage, and a charging indicator (a green bolt above the icon while charging).
- No clock. The StickS3 has no wall-clock source without network or an external RTC, both out of scope; there is intentionally no time display.
- The percentage comes from `M5.Power.getBatteryLevel()` (0–100 for the StickS3 PMIC) and the charge state from `M5.Power.isCharging()`. Reads are throttled to about once per second so the I2C read does not happen every frame.

## Device

- Board: M5Stack StickS3 (ESP32-S3, 8 MB flash, 8 MB PSRAM).
- Display: ST7789P3, 135 × 240, portrait, rotation 0.
- Toolchain: PlatformIO, Arduino framework, M5Unified.

## Out of scope for this version

- An empty navigation shell now exists; extra apps, settings, sound, and network are still out of scope.
