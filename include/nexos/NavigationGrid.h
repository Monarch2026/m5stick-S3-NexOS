#pragma once

#include <M5GFX.h>

namespace nexos {

// Empty app-grid shell for the launcher: a grid of placeholder slots with a
// breathing neon highlight on the selected slot. Pure draw-only (no button or
// state logic); the caller owns `_selected` and passes the halo "breath" each
// frame.
//
// Styled like the splash — same palette, layered neon glow — so the launcher
// reads as the same family even though there are no apps to show yet.
class NavigationGrid {
 public:
  // cols * rows placeholder slots. Call once the target sprite is ready.
  void prepare(LovyanGFX* gfx, int cols, int rows);

  // Slot index to highlight (0..slot_count()-1). Clamped on draw.
  void set_selected(int index);
  int selected() const { return _selected; }
  int slot_count() const { return _cols * _rows; }

  // glow: halo breath, ~0.9..1.0 (same rhythm as NeonLogo).
  void draw(LovyanGFX* gfx, float glow) const;

 private:
  int _cols = 3;
  int _rows = 2;
  int _selected = 0;

  // Layout computed in prepare(). Slot size is fixed; the origin absorbs the
  // leftover space so the grid stays centered.
  int _slotW = 36;
  int _slotH = 36;
  int _gapX = 8;
  int _gapY = 8;
  int _radius = 7;
  int _originX = 0;
  int _originY = 0;
};

}  // namespace nexos
