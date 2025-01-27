#ifndef MCP42010_H
#define MCP42010_H

#include <Arduino.h>

class MCP42010
{
    private:
        // Chip Select, Serial Clock and Data output pins
        int CS, SCK, MOSI;
        // Writing data to the SPI bus
        void SPIWrite(uint8_t data);
    public:
        enum WriteDeviceSelect
        {
            // Write command on Channel 0
            Ch0 = 0b00010001,
            // Write command on Channel 1
            Ch1 = 0b00010010,
            // Write command on both channels
            BothCh = 0b00010011,
        };
        // Initializing the MCP42010 Digital Potentiometer
        MCP42010(int CS, int SCK, int MOSI);
        // Setting the selected potentiometer to the desired value
        void setWiper(uint8_t pot, uint8_t value);
};

#endif // MCP42010_H