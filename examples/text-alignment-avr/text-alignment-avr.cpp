// examples/text-alignment-avr/text-alignment-avr.cpp
//
// NO_ARDUINO+HAL_AVR real-hardware verification: alternates "Hello
// World" between the top-left and bottom-right corners of a 16x2
// display every 2 seconds, exercising setCursor()'s row/column
// addressing and clear() in combination.
#include <BareMetalHAL.h>
#include "HD44780.h"

using namespace BareMetalHAL;

constexpr uint8_t COLS = 16;
const char MESSAGE[] = "Hello World";
constexpr uint8_t MESSAGE_LEN = 11;

int main() {
  timingInit();
  HD44780 lcd(pin(Port::B, 4), pin(Port::B, 5), pin(Port::L, 3),
              pin(Port::L, 4), pin(Port::L, 5), pin(Port::L, 6));
  lcd.begin(COLS, 2);

  while (true) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(MESSAGE);
    delay(2000);

    lcd.clear();
    lcd.setCursor(COLS - MESSAGE_LEN, 1);
    lcd.print(MESSAGE);
    delay(2000);
  }
  return 0;
}
