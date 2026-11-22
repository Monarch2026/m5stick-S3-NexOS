# NexOS

Boot splash firmware for the M5Stack StickS3. The device shows a black screen, a neon diamond with four spikes, the name NexOS, and a loading bar.

The panel is 135 × 240 in portrait. After the bar fills, the splash stays up and the neon halo keeps breathing, with a faint "Press any button" hint. Press any button except power to reach the home screen — an idle page that shows the same mark and name where apps will eventually live.

## What you need

- M5Stack StickS3
- USB-C data cable
- [PlatformIO](https://platformio.org/) (the project was built with PlatformIO Core 6.2)

## Build

From this directory (`m5stick-S3-NexOS`):

```bash
pio run -e m5stack-sticks3
```

The first build downloads the ESP32 platform, M5Unified, and M5GFX.

## Upload

1. Plug the StickS3 into the computer with a data cable.
2. Put it in download mode: hold the power button until the green LED blinks, then release.
3. Upload:

```bash
pio run -e m5stack-sticks3 -t upload
```

4. If the port is ambiguous:

```bash
pio device list
pio run -e m5stack-sticks3 -t upload --upload-port /dev/cu.usbmodemXXXX
```

5. Open the serial log (115200). You should see `NexOS splash 135x240`, then `NexOS splash complete` after about four seconds, and `NexOS: entered home` when you press a button.

```bash
pio device monitor -e m5stack-sticks3
```

Single-click the power button to reset and watch the intro again. Double-click powers the device off.

## What you should see

Hold the stick with the USB port down. A small status row (battery percentage plus a battery icon) sits in the top-right corner on both screens; while charging, a green bolt appears above the icon. There is no clock — network and external RTC are out of scope.

1. A short black beat.
2. A white diamond fades in with a cyan halo and a magenta edge. Spikes grow from the top, right, bottom, and left.
3. **NexOS** appears under the mark.
4. A rounded bar fills from cyan to magenta.
5. The bar stays full, a bright streak moves across it, and the halo breathes.
6. A faint **Press any button** appears below the bar. Press any button except the power button to reach the home screen: the same mark and **NexOS**, no bar, halo still breathing. The status row stays in the top-right corner there too.

If the image is sideways, say so. Rotation is set to `0` in `SplashScreen::begin`.

## Preview without hardware

Before flashing, you can watch the splash on your computer. `preview/render_splash.py` mirrors the C++ timeline and renders 4× frames (needs Pillow):

```bash
pip install pillow
python preview/render_splash.py   # writes preview/splash_<ms>.png
```

The PNGs (`splash_700.png`, `splash_2200.png`, `splash_5200.png`, …) show the diamond, wordmark, and bar at key moments. The home frames (`home_*.png`) also show the status row in the top-right corner — battery percentage with a filled icon, plus a green charging bolt when plugged in. This is a host approximation — confirm on the real device for exact colors and timing.

## Layout

```
include/nexos/     headers
src/main.cpp       boot
src/home/          idle home screen reached from the splash
src/splash/        splash timing and the offscreen frame
src/ui/            logo, wordmark, loading bar, status row
platformio.ini     StickS3 target
requirements.md    what this version must do
architecture.md    how the splash is built
roadmap.md         status and what comes next
```

## Docs

Standing rules are in `Rules.md`.
