#pragma once

#include <M5GFX.h>

namespace nexos {

class LoadingBar {
 public:
  // progress and sheen are 0..1. Pass sheen < 0 to hide the traveling highlight.
  void draw(LovyanGFX* gfx, int x, int y, int width, int height, float progress, float sheen) const;
};

}  // namespace nexos
