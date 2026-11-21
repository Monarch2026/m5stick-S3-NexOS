#include "nexos/LoadingBar.h"

#include <math.h>

#include "nexos/Palette.h"

namespace nexos {

namespace {

bool insideCapsule(int px, int py, int width, int height) {
  float radius = height * 0.5f;
  float dy = (py + 0.5f) - radius;
  if (px + 0.5f >= radius && px + 0.5f < width - radius) {
    return dy * dy <= radius * radius;
  }
  float cx = (px + 0.5f < radius) ? radius : (width - radius);
  float dx = (px + 0.5f) - cx;
  return dx * dx + dy * dy <= radius * radius;
}

Rgb tubeColor(float t) {
  if (t < 0.5f) {
    return mix(Palette::Cyan, Palette::White, t / 0.5f);
  }
  return mix(Palette::White, Palette::Magenta, (t - 0.5f) / 0.5f);
}

}  // namespace

void LoadingBar::draw(LovyanGFX* gfx, int x, int y, int width, int height, float progress,
                      float sheen) const {
  if (width < 4 || height < 2) {
    return;
  }
  if (progress < 0.f) {
    progress = 0.f;
  } else if (progress > 1.f) {
    progress = 1.f;
  }

  int fill = (int)lroundf(progress * width);
  if (progress > 0.f && fill < 1) {
    fill = 1;
  }

  for (int py = 0; py < height; ++py) {
    for (int px = 0; px < width; ++px) {
      if (!insideCapsule(px, py, width, height)) {
        continue;
      }

      Rgb color = Palette::Track;
      if (px < fill) {
        float t = (width <= 1) ? 0.f : (float)px / (float)(width - 1);
        color = tubeColor(t);
        if (px >= fill - 2) {
          color = mix(color, Palette::White, 0.75f);
        }
        if (sheen >= 0.f) {
          float head = sheen * (width + 14.f) - 7.f;
          float distance = fabsf((float)px - head);
          if (distance < 7.f) {
            color = mix(color, Palette::White, (1.f - distance / 7.f) * 0.9f);
          }
        }
      }
      gfx->drawPixel(x + px, y + py, gfx->color565(color.r, color.g, color.b));
    }
  }

  if (fill > 4 && y > 0) {
    uint16_t glow = gfx->color565(Palette::CyanHalo.r, Palette::CyanHalo.g, Palette::CyanHalo.b);
    int glowW = fill - 4;
    gfx->drawFastHLine(x + 2, y - 1, glowW, glow);
    gfx->drawFastHLine(x + 2, y + height, glowW, glow);
  }
}

}  // namespace nexos
