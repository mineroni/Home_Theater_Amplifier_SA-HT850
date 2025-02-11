#include <Arduino.h>

#include "MCP42010.h"


MCP42010::MCP42010(int CS, int SCK, int MOSI)
{
    this->CS = CS;
    this->SCK = SCK;
    this->MOSI = MOSI;

    pinMode(CS, OUTPUT);
    digitalWrite(CS, HIGH);
    pinMode(SCK, OUTPUT);
    digitalWrite(SCK, LOW);
    pinMode(MOSI, OUTPUT);
    digitalWrite(MOSI, LOW);
    setWiper(0, 0);
}

void MCP42010::SPIWrite(uint8_t data)
{
    for (int i = 7; i >= 0; i--)
    {
        digitalWrite(SCK, LOW);
        digitalWrite(MOSI, (data >> i) & 0x01);
        digitalWrite(SCK, HIGH);
    }
    digitalWrite(SCK, LOW);
    digitalWrite(MOSI, LOW);
}

// Converting linear volume level to near logarithmic pot value
uint8_t getPotValueFromVolumeLevel(uint8_t volume)
{
    volume = (volume * 255)/100;            // Scaling 0-100 range to 0-255
    float value = volume * 0.25;            // Setting value before half volume
    if(volume > 127)
        value = volume * 2 - value - 191;   // Setting value after half volume
    return value;
}

void MCP42010::setWiper(uint8_t pot, uint8_t volume)
{
    uint8_t value = getPotValueFromVolumeLevel(volume);
    digitalWrite(CS, LOW);
    SPIWrite(pot);
    SPIWrite(value);
    digitalWrite(CS, HIGH);
}