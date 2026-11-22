#include "nexos/HomeScreen.h"

#include <math.h>

#include <M5Unified.h>

#include "nexos/Palette.h"

namespace nexos {

namespace {

constexpr uint32_t kFrameMs = 33;

}  // namespace

HomeScreen::~HomeScreen() { delete _canvas; }

void HomeScreen::begin(M5GFX& display) {
  _display = &display;
  _display->setRotation(0);
  // Full brightness suits the boot flash; an always-on home is easier on the
  // eyes at a dimmer level. Real brightness control is still to land.
  _display->setBrightness(200);
  _display->fillScreen(TFT_BLACK);

  _canvas = new M5Canvas(_display);
  _canvas->setPsram(true);
  _canvas->setColorDepth(16);
  void* buffer = _canvas->createSprite(_display->width(), _display->height());
  if (buffer == nullptr) {
    _canvas->setPsram(false);
    buffer = _canvas->createSprite(_display->width(), _display->height());
  }

  if (buffer == nullptr) {
    delete _canvas;
    _canvas = nullptr;
    _display->setTextColor(TFT_WHITE);
    _display->setTextDatum(textdatum_t::middle_center);
    _display->setFont(&fonts::FreeSansBold12pt7b);
    _display->drawString(Wordmark::kText, _display->width() / 2, _display->height() / 2);
    Serial.println("NexOS home: sprite alloc failed, static fallback");
    return;
  }

  _wordmark.prepare(_canvas, _display->width() - 12);
  _startMs = millis();
  _lastFrameMs = 0;
  Serial.printf("NexOS home %dx%d\n", _canvas->width(), _canvas->height());
  update();
}

void HomeScreen::update() {
  if (_canvas == nullptr) {
    return;
  }

  uint32_t now = millis();
  if (_lastFrameMs != 0 && (now - _lastFrameMs) < kFrameMs) {
    return;
  }
  _lastFrameMs = now;

  render(now - _startMs);
  _canvas->pushSprite(0, 0);
}

void HomeScreen::render(uint32_t elapsedMs) {
  float elapsed = (float)elapsedMs;
  _canvas->fillSprite(TFT_BLACK);

  // Status row sits in the corner above the mark.
  StatusRow::poll(&_batteryLevel, &_charging);
  _status.draw(_canvas, _batteryLevel, _charging);

  // Same breathing rhythm as the finished splash idle state.
  float breath = 0.90f + 0.10f * sinf(elapsed * 0.0042f);

  int screenW = _canvas->width();
  int screenH = _canvas->height();
  int logoReach = (int)lroundf((NeonLogo::kRadius + NeonLogo::kSpike) * 1.25f);
  int textH = _wordmark.height();
  // No loading bar on the home screen, so the group is a little shorter.
  int groupH = logoReach * 2 + 14 + textH;
  int top = (screenH - groupH) / 2;
  if (top < 8) {
    top = 8;
  }

  int cx = screenW / 2;
  int cy = top + logoReach;
  int nameY = cy + logoReach + 14 + textH / 2;

  // Fully revealed mark: body at full size, spikes out, halo breathing.
  _logo.draw(_canvas, cx, cy, 1.f, 1.f, breath, 1.f);
  _wordmark.draw(_canvas, cx, nameY, 1.f);
}

}  // namespace nexos
