#include <Arduino.h>

#include "tone2.h"
#include "upSound.h"

#include <digitalFastIO.h>
#include <every.h>

void upSound::setup() {
  pinMode(speaker, OUTPUT);
}

void upSound::loop() {

  // Check if note should be stopped
  if (end_time > 0) {
    if (millis() > end_time) {
      upSound::noTone();
    }
  }

  // Play note
#if defined(TONE_GENERATOR) && TONE_GENERATOR == SOFT_TONE
  if (half_period > 0) {
    EVERY(half_period MICROSECONDS) {
      static bool state = true;
      digitalWriteFast(speaker, state);
      state = !state;
    }
  }
#else
  if (freq > 0) {
#if defined(TONE_GENERATOR) && TONE_GENERATOR == TONE2
    ::tone2(speaker, freq);
#else
    ::tone(speaker, freq);
#endif
  }
#endif
}

void upSound::tone(uint16_t freq, uint32_t duration = 0) {
  if (freq > 0 && duration > 0) {
#if defined(TONE_GENERATOR) && TONE_GENERATOR == SOFT_TONE
    this->half_period = 1000000 / freq / 2;
#else
    this->freq = freq;
#endif

    this->end_time = duration ? millis() + duration : 0;
  }
}

void upSound::noTone() {
#if defined(TONE_GENERATOR) && TONE_GENERATOR == SOFT_TONE
  half_period = 0;
  digitalWriteFast(speaker, 0);
#else
  freq = 0;
#if defined(TONE_GENERATOR) && TONE_GENERATOR == TONE2
  ::noTone2(speaker);
#else
  ::noTone(speaker);
#endif
#endif
}
