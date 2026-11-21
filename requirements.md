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

## Device

- Board: M5Stack StickS3 (ESP32-S3, 8 MB flash, 8 MB PSRAM).
- Display: ST7789P3, 135 × 240, portrait, rotation 0.
- Toolchain: PlatformIO, Arduino framework, M5Unified.

## Out of scope for this version

- App launcher, extra apps, settings, sound, and network.
