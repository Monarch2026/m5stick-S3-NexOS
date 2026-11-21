#include "nexos/NeonLogo.h"

#include <math.h>

#include "nexos/Palette.h"

namespace nexos {

namespace {

struct Vec {
  float x;
  float y;
};

Vec mul(Vec v, float s) { return {v.x * s, v.y * s}; }

Vec lerp(Vec a, Vec b, float t) {
  return {a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t};
}

int snap(float v) { return (int)lroundf(v); }

void triangle(LovyanGFX* gfx, float cx, float cy, Vec a, Vec b, Vec c, uint16_t color) {
  gfx->fillTriangle(snap(cx + a.x), snap(cy + a.y), snap(cx + b.x), snap(cy + b.y), snap(cx + c.x),
                    snap(cy + c.y), color);
}

// Union of a diamond and four radial spikes. Spike bases lie on the diamond edges,
// so the silhouette stays continuous instead of looking like glued-on triangles.
void drawMark(LovyanGFX* gfx, float cx, float cy, float s, float spike, float flank, uint16_t color) {
  if (s < 1.5f) {
    return;
  }

  Vec corners[4] = {{0.f, -s}, {s, 0.f}, {0.f, s}, {-s, 0.f}};
  triangle(gfx, cx, cy, corners[0], corners[1], corners[2], color);
  triangle(gfx, cx, cy, corners[0], corners[3], corners[2], color);

  if (spike < 0.5f) {
    return;
  }

  float extend = (s + spike) / s;
  for (int i = 0; i < 4; ++i) {
    Vec tip = corners[i];
    Vec prev = corners[(i + 3) & 3];
    Vec next = corners[(i + 1) & 3];
    triangle(gfx, cx, cy, mul(tip, extend), lerp(tip, prev, flank), lerp(tip, next, flank), color);
  }
}

uint16_t to565(LovyanGFX* gfx, Rgb color) { return gfx->color565(color.r, color.g, color.b); }

}  // namespace

void NeonLogo::draw(LovyanGFX* gfx, int cx, int cy, float scale, float spikeReveal, float glow,
                    float reveal) const {
  if (scale <= 0.02f || reveal <= 0.01f) {
    return;
  }
  if (glow < 0.5f) {
    glow = 0.5f;
  }

  float body = kRadius * scale;
  float spike = kSpike * scale * spikeReveal;
  float bloom = reveal;

  struct Layer {
    float scale;
    float ox;
    float oy;
    Rgb color;
    float alpha;
  };

  // Back to front: wide halo, tighter cyan bloom, magenta fringe, white core.
  const Layer layers[] = {
      {1.46f * glow, 0.f, 0.f, Palette::CyanHalo, 0.95f},
      {1.20f * glow, 0.f, 0.f, Palette::CyanBloom, 1.f},
      {1.02f, 1.f, 1.f, Palette::Magenta, 0.85f},
      {1.f, 0.f, 0.f, Palette::White, 1.f},
  };

  for (const Layer& layer : layers) {
    Rgb color = mix(Palette::Black, layer.color, bloom * layer.alpha);
    drawMark(gfx, cx + layer.ox, cy + layer.oy, body * layer.scale, spike * layer.scale, kFlank,
             to565(gfx, color));
  }
}

}  // namespace nexos
