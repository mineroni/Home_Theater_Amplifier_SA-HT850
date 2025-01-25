#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
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

#endif // UTILS_H