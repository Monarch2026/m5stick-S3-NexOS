#include "nexos/StatusRow.h"

#include <math.h>

#include <M5Unified.h>

#include "nexos/Palette.h"

namespace nexos {

uint32_t StatusRow::sLastPollMs = 0;

namespace {

// The status row is a single compact unit anchored to the top-right corner:
//   [battery icon][NN%]
// with a charging bolt hovering just above the icon. Placed high enough to sit
// clear of the logo, which settles well below on both screens.
constexpr int kRightPad = 8;     // gap from the right edge
constexpr int kIconW = 24;       // battery body width (excl. terminal)
constexpr int kIconH = 11;       // battery body height
constexpr int kBatteryTop = 13;  // top of the battery body, leaves room for the bolt

// How many quick voltage samples to average per read and the gap between them.
constexpr int kSampleCount = 8;
constexpr int kSampleGapMs = 5;

void drawBolt(LovyanGFX* gfx, int cx, int cy) {
  uint16_t c = gfx->color565(Palette::Green.r, Palette::Green.g, Palette::Green.b);
  const float s = 0.9f;
  // Six-vertex lightning bolt (apex at top), fanned from vertex 0 so it can be
  // filled with triangles since LovyanGFX has no polygon fill.
  const int xs[6] = {1, -3, 0, -1, 3, 0};
  const int ys[6] = {-4, 1, 1, 4, -1, -1};
  for (int i = 1; i < 5; ++i) {
    gfx->fillTriangle(
        (int)lroundf(cx + xs[0] * s), (int)lroundf(cy + ys[0] * s),
        (int)lroundf(cx + xs[i] * s), (int)lroundf(cy + ys[i] * s),
        (int)lroundf(cx + xs[i + 1] * s), (int)lroundf(cy + ys[i + 1] * s),
        c);
  }
}

}  // namespace

void StatusRow::poll(int* level, bool* charging) {
  const uint32_t now = millis();
  if (now - sLastPollMs < kPollIntervalMs) {
    return;  // throttled: keep the last known values in *level / *charging
  }
  sLastPollMs = now;

  // The StickS3 PMIC reports battery voltage as a noisy single ADC sample, so a
  // raw reading jitters by a percent or two and can tick backwards. Take a few
  // quick samples and average them for a stable percentage.
  int sum = 0;
  int valid = 0;
  for (int i = 0; i < kSampleCount; ++i) {
    int sample = M5.Power.getBatteryLevel();  // 0..100, or -1 on read error
    if (sample >= 0) {
      sum += sample;
      ++valid;
    }
    if (i + 1 < kSampleCount) {
      delay(kSampleGapMs);
    }
  }
  *level = valid > 0 ? (sum + valid / 2) / valid : -1;  // rounded average

  *charging = M5.Power.isCharging();
}

void StatusRow::draw(LovyanGFX* gfx, int level, bool charging) const {
  if (gfx == nullptr) {
    return;
  }

  const int rightX = gfx->width() - kRightPad;   // right edge of the terminal
  const int iconLeft = rightX - kIconW;
  const int iconTop = kBatteryTop;
  const int cy = iconTop + kIconH / 2;           // vertical center of the row

  uint16_t outline = gfx->color565(Palette::CyanHalo.r, Palette::CyanHalo.g, Palette::CyanHalo.b);

  // Battery body outline plus the terminal nub on the right end.
  gfx->drawRect(iconLeft, iconTop, kIconW, kIconH, outline);
  gfx->drawRect(iconLeft + kIconW, iconTop + (kIconH - 3) / 2, 2, kIconH - 2, outline);

  // Fill proportional to level. Leave room for the border and the terminal.
  int fill = 0;
  if (level > 0 && level <= 100) {
    fill = (int)((float)level * (float)(kIconW - 3) / 100.f);
    if (fill < 1) {
      fill = 1;
    }
  }
  if (fill > 0) {
    uint16_t body = gfx->color565(Palette::Cyan.r, Palette::Cyan.g, Palette::Cyan.b);
    gfx->drawRect(iconLeft + 1, iconTop + 1, fill, kIconH - 2, body);
  }

  // Percentage text to the left of the icon, centered on the row.
  char text[8];
  if (level < 0 || level > 100) {
    snprintf(text, sizeof(text), "--");
  } else {
    snprintf(text, sizeof(text), "%d%%", level);
  }
  gfx->setTextSize(1);
  gfx->setFont(&fonts::FreeSansBold9pt7b);
  const int kGap = 2;
  int textRight = iconLeft - kGap;
  gfx->setTextDatum(textdatum_t::middle_center);
  uint16_t white = gfx->color565(Palette::White.r, Palette::White.g, Palette::White.b);
  gfx->setTextColor(white);
  gfx->drawString(text, textRight - gfx->textWidth(text) / 2, cy);

  // Charging indicator: a bolt hovering above the battery.
  if (charging) {
    drawBolt(gfx, iconLeft + kIconW / 2, kBatteryTop - 6);
  }
}

}  // namespace nexos
