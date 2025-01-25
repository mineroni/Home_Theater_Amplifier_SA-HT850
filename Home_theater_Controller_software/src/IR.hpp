#ifndef IR_H
#define IR_H

#include <Arduino.h>

#include <assert.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

#include "main.h"
#include "utils.hpp"
#include "IRCommands.h"

// Output commands
enum IRCommand
{
  IR_NOOP = 0x00,
  IR_Mute,
  IR_VolumeUp,
  IR_VolumeDown,
  IR_Source,

  IR_UNKNOWN = 0xFF,
};

extern const uint16_t kRecvPin;
extern const uint16_t kCaptureBufferSize;
extern const uint8_t kTimeout;
extern const uint16_t kMinUnknownSize;
extern const uint8_t kTolerancePercentage;
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
extern decode_results results;  // Storing the results
extern uint8_t const irData[][5];

// Setting up IR
void initIR()
{
    // Ignore messages with less than minimum on or off pulses.
    irrecv.setUnknownThreshold(kMinUnknownSize);
    irrecv.setTolerance(kTolerancePercentage);  // Override the default tolerance.
    irrecv.enableIRIn();  // Start the receiver
}

IRCommand handleIR()
{
    // Check if the IR code has been received.
    if (!irrecv.decode(&results)) 
    {
        return IR_NOOP;
    }

    // DEBUG: Check if we got an IR message that was to big for our capture buffer.
    if (results.overflow)
    {
        Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
        return IR_NOOP;
    }
    
    for(size_t i = 0; i < sizeof(irData) / sizeof(*irData); i++)
    {
        if (results.address == irData[i][0])
        {
            for (size_t j = 1; j < sizeof(irData[i]) / sizeof(*(irData[i])); j++)
            {
                if (results.command == irData[i][j])
                {
                    return (IRCommand)j;
                }
            }                
        }
    }
    return IR_UNKNOWN;
}

#endif // IR_H