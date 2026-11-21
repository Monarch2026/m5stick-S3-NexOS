#pragma once

#include <math.h>

namespace nexos {
namespace Easing {

inline float clamp01(float t) {
  if (t < 0.f) {
    return 0.f;
  }
  if (t > 1.f) {
    return 1.f;
  }
  return t;
}

inline float outCubic(float t) {
  t = clamp01(t);
  float remain = 1.f - t;
  return 1.f - remain * remain * remain;
}

inline float inOutCubic(float t) {
  t = clamp01(t);
  if (t < 0.5f) {
    return 4.f * t * t * t;
  }
  float u = -2.f * t + 2.f;
  return 1.f - (u * u * u) / 2.f;
}

// 0 before start, then 0..1 across duration.
inline float window(float elapsedMs, float startMs, float durationMs) {
  if (durationMs <= 0.f) {
    return elapsedMs >= startMs ? 1.f : 0.f;
  }
  return clamp01((elapsedMs - startMs) / durationMs);
}

}  // namespace Easing
}  // namespace nexos
