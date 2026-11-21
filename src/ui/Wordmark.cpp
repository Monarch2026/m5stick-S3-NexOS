#include "nexos/Wordmark.h"

#include <math.h>
#include <string.h>

#include "nexos/Easing.h"
#include "nexos/Palette.h"

namespace nexos {

namespace {

constexpr int kTracking = 2;

void paintLetter(LovyanGFX* gfx, const char* glyph, int x, int y, Rgb color) {
  gfx->setTextColor(gfx->color565(color.r, color.g, color.b));
  gfx->drawString(glyph, x, y);
}

}  // namespace

void Wordmark::prepare(LovyanGFX* gfx, int maxWidth) {
  gfx->setTextSize(1);
  gfx->setTextDatum(textdatum_t::middle_center);
  gfx->setFont(&fonts::FreeSansBold18pt7b);
  if (gfx->textWidth(kText) > maxWidth) {
    gfx->setFont(&fonts::FreeSansBold12pt7b);
  }
  if (gfx->textWidth(kText) > maxWidth) {
    gfx->setFont(&fonts::FreeSansBold9pt7b);
  }
  _height = gfx->fontHeight();
}

void Wordmark::draw(LovyanGFX* gfx, int cx, int cy, float reveal) const {
  if (reveal <= 0.01f) {
    return;
  }

  const int count = (int)strlen(kText);
  int widths[8];
  int total = 0;
  for (int i = 0; i < count; ++i) {
    char glyph[2] = {kText[i], '\0'};
    widths[i] = gfx->textWidth(glyph);
    total += widths[i];
  }
  if (count > 1) {
    total += kTracking * (count - 1);
  }

  int cursor = cx - total / 2;
  for (int i = 0; i < count; ++i) {
    // Spread one overall fade into a short left-to-right stagger.
    float local = Easing::clamp01(reveal * (count + 1.15f) * 0.22f - i * 0.16f);
    if (local > 0.f) {
      char glyph[2] = {kText[i], '\0'};
      int gx = cursor + widths[i] / 2;
      int gy = cy + (int)lroundf((1.f - local) * 5.f);
      paintLetter(gfx, glyph, gx + 1, gy + 1, mix(Palette::Black, Palette::Magenta, local * 0.95f));
      paintLetter(gfx, glyph, gx - 1, gy, mix(Palette::Black, Palette::Cyan, local));
      paintLetter(gfx, glyph, gx, gy, mix(Palette::Black, Palette::White, local));
    }
    cursor += widths[i] + kTracking;
  }
}

}  // namespace nexos
