// #############################################################################
// ### Library for LCD based on Hitachi HD44780                              ###
// ### https://github.com/Strooom                                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "HD44780.h"

HD44780::HD44780(displayType theDisplayType, uint8_t I2CAddress) : theDisplayType(theDisplayType), I2CAddress(I2CAddress)		// Constructor
    {
    Wire.begin();											// Initialize I2C interface
    backLight = false;										// disable backLight by default
    cols = ((uint16_t)theDisplayType & 0xFF00) >> 8;		// nmbr of cols (chars) per line - horizontal
    rows = ((uint16_t)theDisplayType & 0x00FF);				// nmbr of lines - vertical
    chars = rows * cols;
    displayData = new uint8_t[chars];						// dynamically allocate memory, depending on the size of the display..
    clear();
    }

HD44780::~HD44780()											// Destructor
    {
    delete[] displayData;									// release the dynamic memory
    }

void HD44780::initialize()
    {
    delay(50); 							// 0. Wait (at least) 50ms after power-on
    // 1. LCD Reset Sequence for 4-bit bus operation
    writeNibbleLCD(0x30, 0);			// reset-sequence to put the display in 4-bit mode, according to datasheet
    delay(5);
    writeNibbleLCD(0x30, 0);
    writeNibbleLCD(0x30, 0);
    writeNibbleLCD(0x20, 0);
    // 2. LCD Configuration
    writeByteLCD(0x28, 0);				// Function Set : 4-bit mode, 2 lines, 5x7 dot matrix
    writeByteLCD(0x06, 0);				// Entry Mode Set : increment, no shift
    writeByteLCD(0x0C, 0);				// Display On/Off Control : display On, cursor Off, No cursor blink
    writeByteLCD(0x01, 0);				// Display Clear
    delay(2);
    }

void HD44780::clear()
    {
    for (uint8_t i = 0; i < chars; i++)
        {
        displayData[i] = 0x20;			// set all displayData bytes to a 'space'
        }
    }

void HD44780::print(char* string, uint8_t row, uint8_t col)
    {
    // This copies a string to the displayRam,
    // translating row and col to displayRam[] index
    // copies up to end of string (terminating zero), or up to last char on the row

    if ((row < rows) && (col < cols))						// check if the 'position' where we want the text is within the bounds of the display
        {
        uint8_t i = 0;
        while (string[i] && col < cols)						// loop as long as there is string data, AND we are not beyond the last character on a row
            {
            displayData[(row * cols) + col] = string[i];	// copy from string to displayData
            ++i;											// next byte from the string
            ++col;											// next byte in displayData
            }
        }
    }

void HD44780::refresh()
    {
    // Here I need to transfer the display RAM, to the LCD dataRAM
    // this is basically translating addresses
    switch (theDisplayType)
        {
        case displayType::Type16X1:
            writeByteLCD((uint8_t) HD44780Instruction::DDRAMaddressSet | 0x00, 0); ;
            for (uint8_t i = 0; i < 8; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            writeByteLCD((uint8_t) HD44780Instruction::DDRAMaddressSet | 0x40, 0); ;
            for (uint8_t i = 8; i < 16; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            break;

        case displayType::Type16X2:
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x00, 0); ;
            for (uint8_t i = 0; i < 16; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x40, 0); ;
            for (uint8_t i = 16; i < 32; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            break;

        case displayType::Type16X4:
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x00, 0); ;
            for (uint8_t i = 0; i < 16; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x40, 0); ;
            for (uint8_t i = 16; i < 32; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x14, 0); ;
            for (uint8_t i = 32; i < 48; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x54, 0); ;
            for (uint8_t i = 48; i < 64; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            break;

        case displayType::Type20X2:
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x00, 0); ;
            for (uint8_t i = 0; i < 20; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x40, 0); ;
            for (uint8_t i = 20; i < 40; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            break;

        case displayType::Type20X4:
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x00, 0); ;
            for (uint8_t i = 0; i < 20; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x40, 0); ;
            for (uint8_t i = 20; i < 40; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x14, 0); ;
            for (uint8_t i = 40; i < 60; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x54, 0); ;
            for (uint8_t i = 60; i < 80; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            break;

        case displayType::Type40X2:
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x00, 0); ;
            for (uint8_t i = 0; i < 40; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            writeByteLCD((uint8_t)HD44780Instruction::DDRAMaddressSet | 0x40, 0); ;
            for (uint8_t i = 40; i < 80; ++i)
                {
                writeByteLCD(displayData[i], 1);
                }
            break;
        }
    }

void HD44780::writeNibbleLCD(uint8_t iData, boolean commandData)
    {
    // iData : uint8_t of data to be written to LCD
    // RS : RegisterSelect
    //
    // MSB Nibble is written to LCD
    // Mask out these 4 MSBits, Add RegisterSelect (RS) and BackLight
    // Make a pulse by setting E low->high->low
    // All this can be done in 1 I2C transmission of 3 bytes

    uint8_t I2CData = (iData & 0xF0) | (commandData ? RS : 0) | (backLight ? BL : 0);

    Wire.beginTransmission(I2CAddress);
    Wire.write(I2CData);
    Wire.write(I2CData | E);
    Wire.write(I2CData);
    Wire.endTransmission();
    }

void HD44780::writeByteLCD(uint8_t iData, boolean commandData)
    {
    // iData : uint8_t of data to be written to LCD
    // commandData : RegisterSelect : 0 = command, 1 = data
    //
    // Byte is written to LCD as 2 sequential Nibbles, MSNibble first, LSNibble last
    // Mask out these 4 MSBits, Add RegisterSelect and BackLight
    // Make a pulse by setting E low->high->low
    Wire.beginTransmission(I2CAddress);

    uint8_t I2CData = (iData & 0xF0) | (commandData ? RS : 0) | (backLight ? BL : 0);
    Wire.write(I2CData);
    Wire.write(I2CData | E);
    Wire.write(I2CData);
    // Shift lower 4 bits, mask out, Add RegisterSelect and BackLight
    // Make a pulse by setting E low->high->low
    I2CData = ((iData << 4) & 0xF0) | (commandData ? RS : 0) | (backLight ? BL : 0);
    Wire.write(I2CData);
    Wire.write(I2CData | E);
    Wire.write(I2CData);
    Wire.endTransmission();
    // All this can be done in 1 I2C transmission of 6 bytes
    }

