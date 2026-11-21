# Requirements

## Current

NexOS boots on an M5Stack StickS3 and shows a single animated splash screen.

- The screen is black, high contrast, and neon, in the spirit of an Apple boot splash: one centered mark, the product name, and a loading bar.
- The mark is a diamond with four spikes, one leaving each vertex (up, right, down, and left).
- The name is **NexOS**.
- The loading bar fills once, then stays full. A slow highlight keeps moving across it.
- After the intro, the mark stays on screen and the neon halo breathes. There is no home screen yet.
- The splash is the whole firmware. Buttons do not navigate anywhere.

## Device

- Board: M5Stack StickS3 (ESP32-S3, 8 MB flash, 8 MB PSRAM).
- Display: ST7789P3, 135 × 240, portrait, rotation 0.
- Toolchain: PlatformIO, Arduino framework, M5Unified.

## Out of scope for this version

- App launcher, extra apps, settings, sound, and network.
