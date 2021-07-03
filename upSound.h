#pragma once

#include <stdint.h>
#include <uPlugin.h>

#define SOFT_TONE 1
#define TONE2 2

class upSound : public uPlugin {
private:
  uint8_t speaker;
  uint32_t end_time;
#if defined(TONE_GENERATOR) && TONE_GENERATOR == SOFT_TONE
  uint16_t half_period;
#else
  uint16_t freq;
#endif

public:
  upSound(uint8_t speaker) : speaker(speaker) {}

  void setup();
  void loop();

  void tone(uint16_t freq, uint32_t duration = 0);
  void noTone();
};
