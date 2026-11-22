#include "nexos/NavigationGrid.h"

#include <math.h>

#include "nexos/Palette.h"

namespace nexos {

void NavigationGrid::prepare(LovyanGFX* gfx, int cols, int rows) {
  _cols = cols;
  _rows = rows;

  int screenW = gfx->width();
  int screenH = gfx->height();

  // Slot size is fixed; the origin absorbs the leftover space so the grid stays
  // centered horizontally.
  int slotsW = _cols * _slotW + (_cols - 1) * _gapX;
  _originX = (screenW - slotsW) / 2;
  if (_originX < 0) {
    _originX = 0;
  }

  // Center the grid block vertically, but keep it clear of the status row above.
  int blockH = _rows * _slotH + (_rows - 1) * _gapY;
  constexpr int kTopMargin = 36;
  int top = (screenH - blockH) / 2;
  _originY = top > kTopMargin ? top : kTopMargin;
}

void NavigationGrid::set_selected(int index) {
  _selected = index;
}

void NavigationGrid::draw(LovyanGFX* gfx, float glow) const {
  if (gfx == nullptr) {
    return;
  }
  auto to565 = [&](Rgb color) { return gfx->color565(color.r, color.g, color.b); };

  // Map the ~0.9..1.0 breath into a 0..1 pulse for the selected slot.
  float pulse = (glow - 0.9f) / 0.1f;
  if (pulse < 0.f) {
    pulse = 0.f;
  } else if (pulse > 1.f) {
    pulse = 1.f;
  }

  int count = _cols * _rows;
  for (int i = 0; i < count; ++i) {
    int col = i % _cols;
    int row = i / _cols;
    int x = _originX + col * (_slotW + _gapX);
    int y = _originY + row * (_slotH + _gapY);

    if (i == _selected) {
      // Layered neon glow, back to front: halo, body, magenta fringe. Fades
      // toward black with the breath, exactly like NeonLogo's layers.
      Rgb halo = mix(Palette::Black, Palette::CyanBloom, 0.25f + 0.35f * pulse);
      gfx->fillRoundRect(x - 4, y - 4, _slotW + 8, _slotH + 8, _radius + 2, to565(halo));
      Rgb body = mix(Palette::CyanHalo, Palette::White, pulse);
      gfx->fillRoundRect(x + 1, y + 1, _slotW - 2, _slotH - 2, _radius, to565(body));
      Rgb fringe = mix(Palette::Black, Palette::Magenta, 0.6f + 0.3f * pulse);
      gfx->drawRoundRect(x, y, _slotW, _slotH, _radius, to565(fringe));
    } else {
      // Empty placeholder: a faint rounded square with a small center dot.
      Rgb faint = mix(Palette::Black, Palette::CyanHalo, 0.45f);
      gfx->drawRoundRect(x, y, _slotW, _slotH, _radius, to565(faint));
      Rgb dot = mix(Palette::Black, Palette::CyanHalo, 0.35f);
      gfx->fillCircle(x + _slotW / 2, y + _slotH / 2, 2, to565(dot));
    }
  }
}

}  // namespace nexos
