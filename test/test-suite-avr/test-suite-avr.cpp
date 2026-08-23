// test/test-suite-avr/test-suite-avr.cpp
//
// Bare-metal AVR port of ../test-suite/test-suite.ino - same
// register-level assertions, proving the HAL_AVR branch emits the
// identical HD44780 4-bit protocol bit patterns as the Arduino branch.
#include <BareMetalHAL.h>
#include <TestTool.h>
#include "../../src/HD44780.h"
#include <avr/io.h>

using namespace BareMetalHAL;

HD44780 lcd(pin(Port::C, 7), pin(Port::C, 6), pin(Port::C, 4),
            pin(Port::C, 2), pin(Port::C, 0), pin(Port::G, 2));

void testBeginConfiguresPinsAsOutputs(TestInvocation* t) {
  t->setName(F("begin() configures all 6 lines as OUTPUT"));
  lcd.begin(16, 2);
  t->verify((DDRC & _BV(PC7)) != 0, F("RS (PC7) should be OUTPUT"));
  t->verify((DDRC & _BV(PC6)) != 0, F("E (PC6) should be OUTPUT"));
  t->verify((DDRC & _BV(PC4)) != 0, F("D4 (PC4) should be OUTPUT"));
  t->verify((DDRC & _BV(PC2)) != 0, F("D5 (PC2) should be OUTPUT"));
  t->verify((DDRC & _BV(PC0)) != 0, F("D6 (PC0) should be OUTPUT"));
  t->verify((DDRG & _BV(PG2)) != 0, F("D7 (PG2) should be OUTPUT"));
}

void testClearEndsInCommandModeWithExpectedNibble(TestInvocation* t) {
  t->setName(F("clear() ends with RS low and data lines at LCD_CLEARDISPLAY's low nibble"));
  lcd.begin(16, 2);
  lcd.clear();
  t->verify((PORTC & _BV(PC7)) == 0, F("RS should be LOW (command mode)"));
  t->verify((PORTC & _BV(PC4)) != 0, F("d4 should be HIGH"));
  t->verify((PORTC & _BV(PC2)) == 0, F("d5 should be LOW"));
  t->verify((PORTC & _BV(PC0)) == 0, F("d6 should be LOW"));
  t->verify((PORTG & _BV(PG2)) == 0, F("d7 should be LOW"));
}

void testPrintEndsInDataModeWithExpectedNibble(TestInvocation* t) {
  t->setName(F("print('K') ends with RS high and data lines at 'K''s low nibble"));
  lcd.begin(16, 2);
  lcd.print('K');
  t->verify((PORTC & _BV(PC7)) != 0, F("RS should be HIGH (data mode)"));
  t->verify((PORTC & _BV(PC4)) != 0, F("d4 should be HIGH"));
  t->verify((PORTC & _BV(PC2)) != 0, F("d5 should be HIGH"));
  t->verify((PORTC & _BV(PC0)) == 0, F("d6 should be LOW"));
  t->verify((PORTG & _BV(PG2)) != 0, F("d7 should be HIGH"));
}

int main() {
  BareMetalHAL::Uart0::begin(9600);
  BareMetalHAL::timingInit();

  TestFunction tests[] = {
    testBeginConfiguresPinsAsOutputs,
    testClearEndsInCommandModeWithExpectedNibble,
    testPrintEndsInDataModeWithExpectedNibble
  };

  runTestSuiteShowMem(tests);

  while (true) {}
  return 0;
}
