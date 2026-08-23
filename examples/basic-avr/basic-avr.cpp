// examples/basic-avr/basic-avr.cpp
//
// NO_ARDUINO+HAL_AVR usage example: prints "Hello, HD44780!" using
// BareMetalHAL::pin() packed pin identifiers instead of raw Arduino
// pin numbers.
#include <BareMetalHAL.h>
#include "HD44780.h"

using namespace BareMetalHAL;

int main() {
  timingInit();
  HD44780 lcd(pin(Port::C, 7), pin(Port::C, 6), pin(Port::C, 4),
              pin(Port::C, 2), pin(Port::C, 0), pin(Port::G, 2));
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Hello, HD44780!");
  while (true) {}
  return 0;
}
