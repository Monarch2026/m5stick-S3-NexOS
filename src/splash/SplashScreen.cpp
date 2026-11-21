#include "nexos/SplashScreen.h"

#include <math.h>

#include <M5Unified.h>

#include "nexos/Easing.h"
#include "nexos/Palette.h"

namespace nexos {

namespace {

constexpr uint32_t kFrameMs = 33;

constexpr float kBodyStart = 120.f;
constexpr float kBodyDuration = 680.f;
constexpr float kSpikeStart = 460.f;
constexpr float kSpikeDuration = 720.f;
constexpr float kNameStart = 980.f;
constexpr float kNameDuration = 640.f;
constexpr float kBarStart = 1500.f;
constexpr float kBarDuration = 2600.f;

constexpr int kBarHeight = 6;

// Shown once the intro finishes, so it is obvious that a button press goes home.
constexpr const char* kHintText = "Press any button";

// Wait just past the bar fill, then fade the hint in over half a second.
constexpr float kHintStart = kBarStart + kBarDuration + 300.f;
constexpr float kHintDuration = 500.f;

}  // namespace

SplashScreen::~SplashScreen() { delete _canvas; }

void SplashScreen::begin(M5GFX& display) {
  _display = &display;
  _display->setRotation(0);
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
    _staticFallback = true;
    _display->setTextColor(TFT_WHITE);
    _display->setTextDatum(textdatum_t::middle_center);
    _display->setFont(&fonts::FreeSansBold12pt7b);
    _display->drawString(Wordmark::kText, _display->width() / 2, _display->height() / 2);
    Serial.println("NexOS splash: sprite alloc failed, static fallback");
    return;
  }

  _wordmark.prepare(_canvas, _display->width() - 12);
  _startMs = millis();
  _lastFrameMs = 0;
  Serial.printf("NexOS splash %dx%d\n", _canvas->width(), _canvas->height());
  update();
}

void SplashScreen::update() {
  if (_staticFallback || _canvas == nullptr) {
    return;
  }

  uint32_t now = millis();
  if (_lastFrameMs != 0 && (now - _lastFrameMs) < kFrameMs) {
    return;
  }
  _lastFrameMs = now;

  uint32_t elapsed = now - _startMs;
  render(elapsed);
  _canvas->pushSprite(0, 0);

  if (!_bootLogged && elapsed >= (uint32_t)(kBarStart + kBarDuration)) {
    _bootLogged = true;
    Serial.println("NexOS splash complete");
  }
}

void SplashScreen::render(uint32_t elapsedMs) {
  float elapsed = (float)elapsedMs;
  _canvas->fillSprite(TFT_BLACK);

  float body = Easing::outCubic(Easing::window(elapsed, kBodyStart, kBodyDuration));
  // Settle from a slightly smaller mark instead of scaling up from nothing.
  float scale = 0.78f + 0.22f * body;
  float spike = Easing::outCubic(Easing::window(elapsed, kSpikeStart, kSpikeDuration));
  float name = Easing::outCubic(Easing::window(elapsed, kNameStart, kNameDuration));
  float bar = Easing::inOutCubic(Easing::window(elapsed, kBarStart, kBarDuration));

  float breath = 0.90f + 0.10f * sinf(elapsed * 0.0042f);
  float reveal = body;

  int screenW = _canvas->width();
  int screenH = _canvas->height();
  int logoReach = (int)lroundf((NeonLogo::kRadius + NeonLogo::kSpike) * 1.25f);
  int textH = _wordmark.height();
  int groupH = logoReach * 2 + 14 + textH + 18 + kBarHeight;
  int top = (screenH - groupH) / 2;
  if (top < 8) {
    top = 8;
  }

  int cx = screenW / 2;
  int cy = top + logoReach;
  int nameY = cy + logoReach + 14 + textH / 2;
  int barW = (int)(screenW * 0.70f);
  if (barW > screenW - 16) {
    barW = screenW - 16;
  }
  int barX = (screenW - barW) / 2;
  int barY = nameY + textH / 2 + 18;

  _logo.draw(_canvas, cx, cy, scale, spike, breath, reveal);
  _wordmark.draw(_canvas, cx, nameY, name);

  float sheen = -1.f;
  if (bar >= 1.f) {
    float since = elapsed - (kBarStart + kBarDuration);
    sheen = fmodf(since / 2400.f, 1.f);
  }
  _bar.draw(_canvas, barX, barY, barW, kBarHeight, bar, sheen);

  // Once the intro finishes, invite interaction: any button except power goes home.
  float done = Easing::window(elapsed, kHintStart, kHintDuration);
  if (done > 0.f) {
    float breathe = 0.5f + 0.5f * sinf(elapsed * 0.003f);  // 0..1
    float alpha = Easing::clamp01(done * (0.30f + 0.40f * breathe));
    _canvas->setTextSize(1);
    _canvas->setFont(&fonts::FreeSansBold9pt7b);
    _canvas->setTextDatum(textdatum_t::middle_center);
    Rgb hint = mix(Palette::Black, Palette::CyanBloom, alpha);
    // Restore the wordmark font so the next frame's wordmark keeps its size.
    auto savedFont = _canvas->getFont();
    _canvas->drawString(kHintText, screenW / 2, screenH - 16);
    _canvas->setFont(savedFont);
  }
}

}  // namespace nexos
