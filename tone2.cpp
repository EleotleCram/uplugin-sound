
/*
 *  Code based on https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Tone.cpp
 */

#include <Arduino.h>
#include <avr/pgmspace.h>

#define TONE2 2

#if defined(TONE_GENERATOR) && TONE_GENERATOR == TONE2

volatile uint8_t *timer2_pin_port;
volatile uint8_t timer2_pin_mask;

static bool is_timer_initialized = false;

static void assertTimerInitialized(uint8_t pin) {
  if (!is_timer_initialized) {
    is_timer_initialized = true;
    // 8 bit timer
    TCCR2A = 0;
    TCCR2B = 0;
    bitWrite(TCCR2A, WGM21, 1);
    bitWrite(TCCR2B, CS20, 1);
    timer2_pin_port = portOutputRegister(digitalPinToPort(pin));
    timer2_pin_mask = digitalPinToBitMask(pin);
  }
}

// Prescalars {1, 8, 32, 64, 128, 256, 1024}
// Inc factor {   8, 4,  2,  2,   2,   4}
const uint8_t prescalar_increment_factor[] PROGMEM = {8, 4, 2, 2, 2, 4};

void tone2(uint8_t pin, uint16_t frequency) {
  assertTimerInitialized(pin);

  uint32_t ocr = 0;
  uint8_t prescalarbits;
  uint16_t prescalar = 1;
  for (prescalarbits = 0b001; prescalarbits <= 0b111; prescalarbits++) {
    ocr = F_CPU / frequency / 2 / prescalar - 1;

    if (ocr <= 255) {
      break;
    }

    prescalar *= pgm_read_byte(prescalar_increment_factor + prescalarbits - 1);
  }

  // Set the Timer counter registers
  TCCR2B = (TCCR2B & 0b11111000) | prescalarbits;

  // Set the OCR, then turn on the interrupts
  OCR2A = ocr;
  bitWrite(TIMSK2, OCIE2A, 1);
}

void noTone2(uint8_t pin) {
  // Reset Timer 2
  bitWrite(TIMSK2, OCIE2A, 0); // disable interrupt
  TCCR2B = (TCCR2B & 0b11111000) | (1 << CS22);
  OCR2A = 0;
  is_timer_initialized = false;

  // Reset pin
  digitalWrite(pin, 0);
}

ISR(TIMER2_COMPA_vect) {
  // toggle the pin
  *timer2_pin_port ^= timer2_pin_mask;
}

#endif
