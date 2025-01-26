#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <EEPROM.h>
#include "main.h"

inline void initSerial()
{
  Serial.begin(115200);

  #ifdef DEBUG

  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);

  #endif
}

template <typename T>
inline size_t Println(T str)
{
  #ifdef DEBUG

  size_t len = Serial.println(str);
  Serial.flush();
  return len;

  #else
  return 0;
  #endif
}

inline size_t Println()
{
  #ifdef DEBUG

  size_t len = Serial.println();
  Serial.flush();
  return len;

  #else
  return 0;
  #endif
}

template <typename T>
inline size_t Print(T str)
{
  #ifdef DEBUG
  return Serial.print(str);
  #else
  return 0;
  #endif
}

inline void EEPROMUpdate(uint8_t address, uint8_t value)
{
  if(EEPROM.read(address) != value)
  {
    EEPROM.write(address, value);
  } 
}

#endif // UTILS_H