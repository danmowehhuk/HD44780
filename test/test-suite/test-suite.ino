// test/test-suite/test-suite.ino
//
// Register-level verification: after each call, inspects the real AVR
// port registers the driver just drove. Proves the driver emits the
// correct HD44780 4-bit protocol bit patterns.
#include <Arduino.h>
#include <TestTool.h>
#include <HD44780.h>
#include <avr/io.h>

// RS=D30(PC7) E=D31(PC6) D4=D33(PC4) D5=D35(PC2) D6=D37(PC0) D7=D39(PG2).
HD44780 lcd(30, 31, 33, 35, 37, 39);

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
  // LCD_CLEARDISPLAY = 0x01; its low nibble (0001) is the last nibble
  // sent - d4=1, d5=0, d6=0, d7=0.
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
  // 'K' = 0x4B; its low nibble (1011) is the last nibble sent -
  // d4=1, d5=1, d6=0, d7=1.
  t->verify((PORTC & _BV(PC7)) != 0, F("RS should be HIGH (data mode)"));
  t->verify((PORTC & _BV(PC4)) != 0, F("d4 should be HIGH"));
  t->verify((PORTC & _BV(PC2)) != 0, F("d5 should be HIGH"));
  t->verify((PORTC & _BV(PC0)) == 0, F("d6 should be LOW"));
  t->verify((PORTG & _BV(PG2)) != 0, F("d7 should be HIGH"));
}

void testPrintFlashStrEndsInDataModeWithExpectedNibble(TestInvocation* t) {
  t->setName(F("print(F(\"K\")) ends with RS high and data lines at 'K''s low nibble"));
  lcd.begin(16, 2);
  lcd.print(F("K"));
  // 'K' = 0x4B; its low nibble (1011) is the last nibble sent -
  // d4=1, d5=1, d6=0, d7=1.
  t->verify((PORTC & _BV(PC7)) != 0, F("RS should be HIGH (data mode)"));
  t->verify((PORTC & _BV(PC4)) != 0, F("d4 should be HIGH"));
  t->verify((PORTC & _BV(PC2)) != 0, F("d5 should be HIGH"));
  t->verify((PORTC & _BV(PC0)) == 0, F("d6 should be LOW"));
  t->verify((PORTG & _BV(PG2)) != 0, F("d7 should be HIGH"));
}

void setup() {
  Serial.begin(9600);
  TestFunction tests[] = {
    testBeginConfiguresPinsAsOutputs,
    testClearEndsInCommandModeWithExpectedNibble,
    testPrintEndsInDataModeWithExpectedNibble,
    testPrintFlashStrEndsInDataModeWithExpectedNibble
  };
  runTestSuiteShowMem(tests);
}

void loop() {}
