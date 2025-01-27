#ifndef IRCOMMNADS_H
#define IRCOMMNADS_H

#include <Arduino.h>

// Addresses and commands for the IR remotes
uint8_t const irData[][5] = 
{//     NAME                    ADRESS,     Mute_Command,   VolumeUp_Command,   VolumeDown_Command,     Source_Command
        /*Hitachi*/             {0x03,          0x0D,           0x10,                   0x11,               0x38},
        /*Vestel_Tin*/          {0x01,          0x0D,           0x10,                   0x11,               0x3F},
};

#endif // IRCOMMNADS_H