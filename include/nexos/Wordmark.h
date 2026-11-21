#pragma once

#include <M5GFX.h>

namespace nexos {

class Wordmark {
 public:
  static constexpr const char* kText = "NexOS";

  // Picks a bold sans that fits maxWidth. Call once the target is ready.
  void prepare(LovyanGFX* gfx, int maxWidth);
  int height() const { return _height; }

  // reveal 0..1 fades the letters in with a short stagger.
  void draw(LovyanGFX* gfx, int cx, int cy, float reveal) const;

 private:
  int _height = 16;
};

}  // namespace nexos
