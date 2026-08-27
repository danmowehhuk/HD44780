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

#else

#include <BareMetalHAL.h>

using namespace BareMetalHAL;

namespace {
  // HD44780 command opcodes this driver uses.
  constexpr uint8_t LCD_CLEARDISPLAY   = 0x01;
  constexpr uint8_t LCD_ENTRYMODESET   = 0x04;
  constexpr uint8_t LCD_DISPLAYCONTROL = 0x08;
  constexpr uint8_t LCD_FUNCTIONSET    = 0x20;
  constexpr uint8_t LCD_SETDDRAMADDR   = 0x80;

  constexpr uint8_t LCD_ENTRYLEFT           = 0x02;
  constexpr uint8_t LCD_ENTRYSHIFTDECREMENT = 0x00;

  constexpr uint8_t LCD_DISPLAYON = 0x04;
  constexpr uint8_t LCD_CURSORON  = 0x02;
  constexpr uint8_t LCD_BLINKON   = 0x01;

  constexpr uint8_t LCD_4BITMODE = 0x00;
  constexpr uint8_t LCD_2LINE    = 0x08;
  constexpr uint8_t LCD_5x8DOTS  = 0x00;

  // 2-row displays only - a 4-row display needs a longer offset table.
  constexpr uint8_t ROW_OFFSETS[2] = {0x00, 0x40};
}

HD44780::HD44780(uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5,
                  uint8_t d6, uint8_t d7)
  : _rsPin(rs), _enablePin(enable), _d4Pin(d4), _d5Pin(d5),
    _d6Pin(d6), _d7Pin(d7), _displayControl(0) {}

void HD44780::begin(uint8_t cols, uint8_t rows) {
  (void)cols;
  pinMode(_rsPin, OUTPUT);
  pinMode(_enablePin, OUTPUT);
  pinMode(_d4Pin, OUTPUT);
  pinMode(_d5Pin, OUTPUT);
  pinMode(_d6Pin, OUTPUT);
  pinMode(_d7Pin, OUTPUT);

  // Datasheet-mandated power-on wait (min 40ms after Vcc rises above
  // 2.7V) - Hitachi HD44780 datasheet page 45/46, figure 24,
  // reproduced from Adafruit_LiquidCrystal::begin()'s own comment.
  delayMicroseconds(50000);

  digitalWrite(_rsPin, LOW);
  digitalWrite(_enablePin, LOW);

  // Put the display into 4-bit mode: three raw nibble writes per the
  // datasheet's power-on sequence (figure 24) - the display doesn't
  // know its own mode yet at this point, so command()/send() (which
  // assume 4-bit mode) can't be used here.
  write4bits(0x03);
  delayMicroseconds(4500);
  write4bits(0x03);
  delayMicroseconds(4500);
  write4bits(0x03);
  delayMicroseconds(150);
  write4bits(0x02);

  uint8_t displayFunction = LCD_4BITMODE | LCD_5x8DOTS;
  if (rows > 1) displayFunction |= LCD_2LINE;
  command(LCD_FUNCTIONSET | displayFunction);

  _displayControl = LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displayControl);

  clear();

  command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
}

void HD44780::clear() {
  command(LCD_CLEARDISPLAY);
  delayMicroseconds(2000);
}

void HD44780::setCursor(uint8_t col, uint8_t row) {
  if (row > 1) row = 1;
  command(LCD_SETDDRAMADDR | (col + ROW_OFFSETS[row]));
}

void HD44780::cursor() {
  _displayControl |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displayControl);
}

void HD44780::noCursor() {
  _displayControl &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displayControl);
}

void HD44780::blink() {
  _displayControl |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displayControl);
}

void HD44780::noBlink() {
  _displayControl &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displayControl);
}

void HD44780::print(char c) { write((uint8_t)c); }

void HD44780::print(const char* str) {
  while (*str) write((uint8_t)*str++);
}

void HD44780::print(const FlashStr* str) {
  const char* flashPtr = reinterpret_cast<const char*>(str);
  char c;
  while ((c = readByte(flashPtr++)) != '\0') write((uint8_t)c);
}

void HD44780::command(uint8_t value) { send(value, LOW); }

void HD44780::write(uint8_t value) { send(value, HIGH); }

void HD44780::send(uint8_t value, uint8_t rsLevel) {
  digitalWrite(_rsPin, rsLevel);
  write4bits(value >> 4);
  write4bits(value);
}

void HD44780::write4bits(uint8_t nibble) {
  digitalWrite(_d4Pin, (nibble >> 0) & 0x01);
  digitalWrite(_d5Pin, (nibble >> 1) & 0x01);
  digitalWrite(_d6Pin, (nibble >> 2) & 0x01);
  digitalWrite(_d7Pin, (nibble >> 3) & 0x01);
  pulseEnable();
}

void HD44780::pulseEnable() {
  digitalWrite(_enablePin, LOW);
  delayMicroseconds(1);
  digitalWrite(_enablePin, HIGH);
  delayMicroseconds(1);
  digitalWrite(_enablePin, LOW);
  delayMicroseconds(100);
}

#endif
