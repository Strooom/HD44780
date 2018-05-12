#include <Arduino.h>
#include <Wire.h>
#include "HD44780.h"

HD44780 theDisplay(displayType::Type16X2, 0x3F);
// This creates 1 global instance of the HD44780 class, named (as an example) 'theDisplay'
// displayType is 16x2, see HD44780.h for an enumeration of all supported types
// The display is connected via I2C, at I2C-address 0x3F. If you don't know the I2C address of your hardware, use https://playground.arduino.cc/Main/I2cScanner

void setup()
  {
  theDisplay.initialize();                    // initialize the display, needed after power-on
  theDisplay.print("Hello World!", 0, 0);     // write string to line 0, column 0
  theDisplay.print("16x2LCD", 1, 0);          // write string to line 1, column 0
  theDisplay.print("I2C@0x3F", 1, 8);         // write string to line 1, column 8
  theDisplay.refresh();                       // copy all text from microController to LCD
  }

void loop()
  {
  }
