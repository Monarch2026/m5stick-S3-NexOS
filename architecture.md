# Architecture

NexOS is an Arduino firmware built with PlatformIO for the M5StickS3. M5Unified owns the panel, backlight, and buttons. The app draws into a full-frame sprite, then pushes that sprite to the panel.

## Boot

`src/main.cpp` configures M5Unified with the speaker, microphone, and IMU left off, then hands the display to `SplashScreen`. The main loop only calls `M5.update()` and `SplashScreen::update()`.

`SplashScreen` locks rotation 0 (135 × 240, portrait) and full backlight. It allocates one 16-bit sprite in PSRAM (135 × 240 × 2 = 64,800 bytes) and falls back to internal RAM, then to a static "NexOS" string if both allocations fail.

## Splash

Timing lives in `src/splash/SplashScreen.cpp`. Each frame is paced at about 30 fps and is a pure function of milliseconds since boot:

| Time | What happens |
| --- | --- |
| 0.12–0.80 s | Diamond eases in from 78% size and fades up from black |
| 0.46–1.18 s | Four spikes extend from the diamond vertices |
| 0.98–1.62 s | "NexOS" fades in, letter by letter |
| 1.50–4.10 s | Loading bar fills, cyan through white to magenta |
| After 4.10 s | Bar stays full, a highlight travels across it, and the halo breathes |

Drawing is split so each piece can be reused by later screens:

- `NeonLogo` draws the diamond and spikes. Each spike base sits on the two edges beside a vertex, and the tip extends radially, so the outline is one shape. Layers, back to front: dark cyan halo, cyan bloom, a 1 px magenta fringe, white core. Only the halo and bloom change size with the breath.
- `Wordmark` chooses FreeSans Bold 18 pt, 12 pt, or 9 pt so the name fits the width. Letters are drawn magenta, cyan, then white for a neon fringe.
- `LoadingBar` fills a capsule pixel by pixel: dark track, neon gradient, white leading edge, and an optional sheen.

## Later apps

New screens should follow the same split: a screen object that owns timing, and small drawers under `src/ui` that only paint. The sprite stays the place frames are composed so the panel never shows a partial draw.
