#include "nexos/HomeScreen.h"

#include <M5Unified.h>

namespace nexos {

namespace {

constexpr uint32_t kFrameMs = 33;

}  // namespace

HomeScreen::~HomeScreen() { delete _canvas; }

void HomeScreen::begin(M5GFX& display) {
  _display = &display;
  _display->setRotation(0);
  // A dimmer level suits an always-on resting screen. Real brightness control
  // is still to land.
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
    // Pure black has nothing to draw; a plain fill is the whole screen.
    Serial.println("NexOS home: sprite alloc failed, pure-black fallback");
    return;
  }

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
  (void)elapsedMs;
  // The home screen is intentionally just a black background.
  _canvas->fillSprite(TFT_BLACK);
}

}  // namespace nexos
