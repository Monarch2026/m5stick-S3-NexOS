# Roadmap

## v0.1 — Splash (current)

Status: **Confirmed on hardware.** Firmware written, compiles cleanly, matches M5Stack's official StickS3 PlatformIO config, verified visually with a host-side preview (`preview/render_splash.py`, 4× render), and running correctly on the physical StickS3.

- Animated neon splash: four-spike diamond, "NexOS", loading bar.
- Portrait 135 × 240 framebuffer in PSRAM (8 MB octal), with internal-RAM fallback.
- Stays on the finished splash after the bar fills. No launcher yet.

### Verified off-device

- Builds clean: `pio run -e m5stack-sticks3` → SUCCESS (RAM 7%, Flash 17%).
- `platformio.ini` matches the official StickS3 reference exactly (`qio_opi`, `-DBOARD_HAS_PSRAM`, `-mfix-esp32-psram-cache-issue`). The generic devkitc-1 board JSON reports "No PSRAM", but those flags override it at build time.
- `preview/render_splash.py` mirrors the C++ timeline and renders 540 × 960 frames; see `preview/splash_*.png`.

### Confirm on hardware (DONE 2026-11-21)

All boxes checked on the physical StickS3:

- [x] Boot log prints `NexOS splash 135x240` then `NexOS splash complete` (~4 s).
- [x] Diamond eases in from ~78%, spikes grow, "NexOS" fades letter by letter.
- [x] Bar fills cyan → white → magenta once, then stays full.
- [x] A highlight travels across the full bar and the halo breathes (no static freeze).
- [x] Image is upright when held USB-port-down; text is readable at arm's length.
- [x] No flicker or artifacts (sprite is double-buffered, so none expected).

If PSRAM fails to allocate you'll see `NexOS splash: sprite alloc failed, static fallback` and a plain "NexOS" string instead of the animation — that points to a PSRAM/board-config issue.

## v0.2 — Home screen (current)

Status: **Builds clean; confirm on hardware.**

- Added an empty home screen reached by pressing **any button except the power button** after the boot splash finishes. The power button never enters it.
- `HomeScreen` reuses `NeonLogo` + `Wordmark`: a fully revealed mark and "NexOS" with a breathing halo, no loading bar, dimmer than the splash for an always-on screen.
- Boot is now a small two-state machine in `src/main.cpp`; the splash keeps its intro and adds a faint "Press any button" hint once finished.
- Host preview (`preview/render_splash.py`) renders the hint frame and home frames; see `preview/splash_4700.png` and `preview/home_*.png`.

## Chosen ideas (pending build)

These three were chosen and are ready to implement, in this order:

1. **Status row** — battery % + clock in a corner, shown across the splash and home. Small; no extra hardware.
2. **Themes / accent colors** — swap the neon accent palette via a small settings screen. Small; no extra hardware.
3. **Tiny apps pack** — 2–3 small apps such as stopwatch, dice, and compass. The compass needs the IMU turned on; stopwatch and dice don't. Large; needs the app shell below first.

## Next

- App shell: an app grid on the home screen with button A / C to move and select-to-open. Foundation for the tiny apps pack above. (Medium)
- First real apps (still to be chosen).
- Return navigation: leaving the home screen back to the splash (entry is one-way so far).

## Later

- Brightness control. Full brightness is correct for the splash and too much for an always-on home screen.
