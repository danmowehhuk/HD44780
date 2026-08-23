# HD44780

Driver for a Hitachi HD44780-compatible character LCD in 4-bit mode (no
R/W pin).

- **Arduino path**: wraps [Adafruit_LiquidCrystal](https://github.com/adafruit/Adafruit_LiquidCrystal).
- **`NO_ARDUINO`+`HAL_AVR` path**: reimplements the same 4-bit protocol
  natively against [BareMetalHAL](https://github.com/danmowehhuk/BareMetalHAL).
