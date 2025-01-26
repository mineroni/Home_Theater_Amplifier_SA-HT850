#include <Arduino.h>
#include <EEPROM.h>

#include "Volume.h"
#include "utils.hpp"

Volume::Volume()
{
    // Starting EEPROM
    EEPROM.begin(ChannelIDCount);
    
    // Reading EEPROM values to initialize volume and offsets
    volume = EEPROM.read(FRONT);
    // If value is invalid, set it to 0
    if (volume > VolumeMax)
        volume = 0;
    for(uint8_t i = 1; i < ChannelIDCount; i++)
    {
        offsets[i] = EEPROM.read(i);
        // If value is invalid, set it to 0
        if (offsets[i] > MaxOffset || offsets[i] < -MaxOffset)
            offsets[i] = 0;
    }
    muted = false;
}

uint8_t Volume::getVolume()
{
    if (muted) return 0;
    return volume;
}

uint8_t Volume::getSurroundVolume()
{
    if (muted) return 0;
    return (-offsets[SURROUND] > volume) ? 0 : volume + offsets[SURROUND];
}

uint8_t Volume::getSubVolume()
{
    if (muted) return 0;
    return (-offsets[SUBWOOFER] > volume) ? 0 : volume + offsets[SUBWOOFER];
}

uint8_t Volume::getCenterVolume()
{
    if (muted) return 0;
    return (-offsets[CENTER] > volume) ? 0 : volume + offsets[CENTER];
}

int8_t* Volume::getOffsets()
{
    return offsets;
}

void Volume::changeVolume(int8_t change)
{
    if (muted)
    {
        muted = false;
        return;
    }

    volume = (-change > volume) ? 0 : volume + change;
    if(volume > VolumeMax)
    {
        volume = VolumeMax;
    }
    EEPROMUpdate(FRONT, volume);
    EEPROM.commit();
}

void Volume::changeSurroundOffset(int8_t change)
{
    offsets[SURROUND] += change;
    if(offsets[SURROUND] > MaxOffset)
    {
        offsets[SURROUND] = MaxOffset;
    }
    else if(offsets[SURROUND] < -MaxOffset)
    {
        offsets[SURROUND] = -MaxOffset;
    }
    EEPROMUpdate(SURROUND, offsets[SURROUND]);
    EEPROM.commit();
}

void Volume::changeSubOffset(int8_t change)
{
    offsets[SUBWOOFER] += change;
    if(offsets[SUBWOOFER] > MaxOffset)
    {
        offsets[SUBWOOFER] = MaxOffset;
    }
    else if(offsets[SUBWOOFER] < -MaxOffset)
    {
        offsets[SUBWOOFER] = -MaxOffset;
    }
    EEPROMUpdate(SUBWOOFER, offsets[SUBWOOFER]);
    EEPROM.commit();
}

void Volume::changeCenterOffset(int8_t change)
{
    offsets[CENTER] += change;
    if(offsets[CENTER] > MaxOffset)
    {
        offsets[CENTER] = MaxOffset;
    }
    else if(offsets[CENTER] < -MaxOffset)
    {
        offsets[CENTER] = -MaxOffset;
    }
    EEPROMUpdate(CENTER, offsets[CENTER]);
    EEPROM.commit();
}

void Volume::mute()
{
    muted = !muted;
}

void Volume::resetOffsets()
{
    for(uint8_t i = 0; i < ChannelIDCount; i++)
    {
        offsets[i] = 0;
        EEPROMUpdate(i, 0);
    }
    EEPROM.commit();
}

void Volume::printVolumeStatus()
{
    String status = "Volume: ";
    status += getVolume();
    status += ", Surround volume: ";
    status += getSurroundVolume();
    status += ", Subwoofer volume: ";
    status += getSubVolume();
    status += ", Center volume: ";
    status += getCenterVolume();
    status += "\nSurround offset: ";
    status += offsets[SURROUND];
    status += ", Subwoofer offset: ";
    status += offsets[SUBWOOFER];
    status += ", Center offset: ";
    status += offsets[CENTER];
    Println(status);
}