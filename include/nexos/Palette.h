#pragma once

#include <stdint.h>

namespace nexos {

struct Rgb {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

inline Rgb mix(Rgb from, Rgb to, float t) {
  if (t < 0.f) {
    t = 0.f;
  } else if (t > 1.f) {
    t = 1.f;
  }
  return {
      (uint8_t)(from.r + (to.r - from.r) * t + 0.5f),
      (uint8_t)(from.g + (to.g - from.g) * t + 0.5f),
      (uint8_t)(from.b + (to.b - from.b) * t + 0.5f),
  };
}

namespace Palette {

constexpr Rgb Black{0, 0, 0};
constexpr Rgb White{255, 255, 255};
constexpr Rgb Cyan{0, 255, 255};
constexpr Rgb CyanBloom{0, 186, 230};
constexpr Rgb CyanHalo{0, 52, 86};
constexpr Rgb Magenta{255, 0, 196};
constexpr Rgb Track{32, 34, 46};

}  // namespace Palette
}  // namespace nexos
