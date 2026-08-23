#ifndef HD44780_H
#define HD44780_H

#include <stdint.h>
#include "hal/FlashStr.h"

#ifndef NO_ARDUINO
#include <Adafruit_LiquidCrystal.h>
#endif

// Driver for a Hitachi HD44780-compatible character LCD in 4-bit mode,
// no R/W pin. Wraps Adafruit_LiquidCrystal on the Arduino path;
// reimplements the same protocol natively against BareMetalHAL on
// NO_ARDUINO+HAL_AVR.
class HD44780 {
  public:
    // rs/enable/d4/d5/d6/d7 mean different things per branch: a raw
    // Arduino pin number on the Arduino path, a
    // BareMetalHAL::pin(Port, bit) packed value on NO_ARDUINO+HAL_AVR.
    HD44780(uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5,
            uint8_t d6, uint8_t d7);

    void begin(uint8_t cols, uint8_t rows);
    void clear();
    // NO_ARDUINO+HAL_AVR: row is clamped to 0/1 (2-row displays only).
    void setCursor(uint8_t col, uint8_t row);
    void cursor();
    void noCursor();
    void blink();
    void noBlink();
    void print(char c);
    void print(const char* str);
    void print(const FlashStr* str);

  private:
#ifndef NO_ARDUINO
    Adafruit_LiquidCrystal _lcd;
#else
    uint8_t _rsPin, _enablePin, _d4Pin, _d5Pin, _d6Pin, _d7Pin;
    uint8_t _displayControl;

    void command(uint8_t value);
    void write(uint8_t value);
    void send(uint8_t value, uint8_t rsLevel);
    void write4bits(uint8_t nibble);
    void pulseEnable();
#endif
};

#endif
