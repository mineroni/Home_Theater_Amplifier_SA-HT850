#ifndef VOLUME_H
#define VOLUME_H

#include <Arduino.h>

#define VolumeMax               (100)
#define MaxOffset               (20)

class Volume
{
    public:
        // Necessary
        Volume();
        enum ChannelID
        {
            FRONT = 0,
            SURROUND = 1,
            SUBWOOFER = 2,
            CENTER = 3,
            ChannelIDCount
        };

        // Getters
        uint8_t getVolume();
        uint8_t getSurroundVolume();
        uint8_t getSubVolume();
        uint8_t getCenterVolume();
        int8_t* getOffsets();

        // Setters
        void changeVolume(int8_t change);
        void changeSurroundOffset(int8_t change);
        void changeSubOffset(int8_t change);
        void changeCenterOffset(int8_t change);
        void mute();
        void resetOffsets();
        void printVolumeStatus();
    private:
        // Internal fields
        uint8_t volume;
        bool muted;
        int8_t offsets[ChannelIDCount];
};

#endif