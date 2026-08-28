// Shared pin defaults for HD44780 examples.
//
// Pin values below may be supplied by an external project instead of the
// defaults here — e.g. via `-include Pins.h` on the build command line,
// where that header defines OVERRIDE_PINS plus each of the names below.
#ifndef OVERRIDE_PINS
  #ifndef LCD_RS_PIN
  #define LCD_RS_PIN 30
  #endif
  #ifndef LCD_E_PIN
  #define LCD_E_PIN  31
  #endif
  #ifndef LCD_D4_PIN
  #define LCD_D4_PIN 33
  #endif
  #ifndef LCD_D5_PIN
  #define LCD_D5_PIN 35
  #endif
  #ifndef LCD_D6_PIN
  #define LCD_D6_PIN 37
  #endif
  #ifndef LCD_D7_PIN
  #define LCD_D7_PIN 39
  #endif
#endif
