#include "HD44780.h"

#ifndef NO_ARDUINO

HD44780::HD44780(uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5,
                  uint8_t d6, uint8_t d7)
  : _lcd(rs, enable, d4, d5, d6, d7) {}

void HD44780::begin(uint8_t cols, uint8_t rows) { _lcd.begin(cols, rows); }
void HD44780::clear() { _lcd.clear(); }
void HD44780::setCursor(uint8_t col, uint8_t row) { _lcd.setCursor(col, row); }
void HD44780::cursor() { _lcd.cursor(); }
void HD44780::noCursor() { _lcd.noCursor(); }
void HD44780::blink() { _lcd.blink(); }
void HD44780::noBlink() { _lcd.noBlink(); }
void HD44780::print(char c) { _lcd.print(c); }
void HD44780::print(const char* str) { _lcd.print(str); }
void HD44780::print(const FlashStr* str) { _lcd.print(str); }

#endif
