#pragma once

#include <M5GFX.h>

namespace nexos {

// Diamond mark with one spike on each vertex (up, right, down, left).
class NeonLogo {
 public:
  static constexpr float kRadius = 26.f;
  static constexpr float kSpike = 15.f;
  // How far down each diamond edge the spike base sits. 0 is the vertex.
  static constexpr float kFlank = 0.30f;

  // scale: body size, 0..1. spikeReveal: spike length, 0..1.
  // glow: bloom size around 1. reveal: 0..1 fades every layer in from black.
  void draw(LovyanGFX* gfx, int cx, int cy, float scale, float spikeReveal, float glow,
            float reveal) const;
};

}  // namespace nexos
