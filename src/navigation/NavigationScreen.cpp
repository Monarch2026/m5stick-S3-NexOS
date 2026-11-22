#include "nexos/NavigationScreen.h"

#include <math.h>

#include <M5Unified.h>

#include "nexos/Palette.h"

namespace nexos {

namespace {

constexpr uint32_t kFrameMs = 33;

}  // namespace

NavigationScreen::~NavigationScreen() { delete _canvas; }

void NavigationScreen::begin(M5GFX& display) {
  _display = &display;
  _display->setRotation(0);
  // Full brightness matches the splash's neon pop for the launcher.
  _display->setBrightness(255);
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
    Serial.println("NexOS nav: sprite alloc failed");
    return;
  }

  // 3 x 2 grid of placeholder slots.
  _grid.prepare(_canvas, 3, 2);
  _grid.set_selected(0);
  _startMs = millis();
  _lastFrameMs = 0;
  Serial.printf("NexOS nav %dx%d\n", _canvas->width(), _canvas->height());
  update();
}

void NavigationScreen::update() {
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

void NavigationScreen::advance() {
  if (_canvas == nullptr) {
    return;
  }
  int count = _grid.slot_count();
  if (count <= 1) {
    return;
  }
  _grid.set_selected((_grid.selected() + 1) % count);
}

void NavigationScreen::render(uint32_t elapsedMs) {
  float elapsed = (float)elapsedMs;
  _canvas->fillSprite(TFT_BLACK);

  // Status row sits in the corner, like the splash.
  StatusRow::poll(&_batteryLevel, &_charging);
  _status.draw(_canvas, _batteryLevel, _charging);

  // Same breathing rhythm as the finished splash idle state.
  float breath = 0.90f + 0.10f * sinf(elapsed * 0.0042f);
  _grid.draw(_canvas, breath);
}

}  // namespace nexos
