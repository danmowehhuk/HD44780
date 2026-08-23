// examples/basic/basic.ino
//
// Minimal usage example: prints "Hello, HD44780!" on a 16x2 display.
#include <HD44780.h>

HD44780 lcd(30, 31, 33, 35, 37, 39);

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Hello, HD44780!");
}

void loop() {}
