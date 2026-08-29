// examples/basic/basic.ino
//
// Minimal usage example: prints "Hello, HD44780!" on a 16x2 display.
#include <HD44780.h>
#include "PinSetup.h"

HD44780 lcd(LCD_RS_PIN, LCD_E_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Hello, HD44780!");
}

void loop() {}
