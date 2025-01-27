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

void MCP42010::setWiper(uint8_t pot, uint8_t value)
{
    digitalWrite(CS, LOW);
    SPIWrite(pot);
    SPIWrite(value);
    digitalWrite(CS, HIGH);
}