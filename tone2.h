
#pragma once

#if defined(TONE_GENERATOR) && TONE_GENERATOR == TONE2

void tone2(uint8_t pin, uint16_t frequency);
void noTone2(uint8_t pin);

#endif
