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

## Next

- First GitHub version tagged and pushed (2026-11-21).
- Home screen that the splash hands off to when the bar completes.
- App shell: a list of apps, button A / button B to move and open.
- First real apps (still to be chosen).
- Shared status row (battery, time) once more than one screen exists.
- Brightness control. Full brightness is correct for the splash and too much for an always-on home screen.

## Later

- Home screen that the splash hands off to when the bar completes.
- App shell: a list of apps, button A / button B to move and open.
- First real apps (still to be chosen).
- Shared status row (battery, time) once more than one screen exists.
- Brightness control. Full brightness is correct for the splash and too much for a always-on home screen.
