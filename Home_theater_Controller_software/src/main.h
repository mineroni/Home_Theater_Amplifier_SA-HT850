#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <IRutils.h>

#include "VolumeController/Volume.h"
#include "DigitalPotDriver/MCP42010.h"

#define DEBUG


// ======================== Defining output pins =======================

const uint8_t SPI_SCK = 12;
const uint8_t SPI_MOSI = 14;
const uint8_t SPI_CS0 = 2;
const uint8_t SPI_CS1 = 0;
const uint8_t SPI_CS2 = 4;

// ==================== start of TUNEABLE PARAMETERS ====================
// IR Volume change step
const uint8_t volumeChangeStep = 5;

// Delay in ms between IR captures
const uint16_t captureDelayIR = 200;

// IR receiver pin
const uint16_t kRecvPin = 13;

// As this program is a special purpose capture/decoder, let us use a larger
// than normal buffer so we can handle Air Conditioner remote codes.
const uint16_t kCaptureBufferSize = 1024;

// kTimeout is the Nr. of milli-Seconds of no-more-data before we consider a
// message ended.
// This parameter is an interesting trade-off. The longer the timeout, the more
// complex a message it can capture. e.g. Some device protocols will send
// multiple message packets in quick succession, like Air Conditioner remotes.
// Air Coniditioner protocols often have a considerable gap (20-40+ms) between
// packets.
// The downside of a large timeout value is a lot of less complex protocols
// send multiple messages when the remote's button is held down. The gap between
// them is often also around 20+ms. This can result in the raw data be 2-3+
// times larger than needed as it has captured 2-3+ messages in a single
// capture. Setting a low timeout value can resolve this.
// So, choosing the best kTimeout value for your use particular case is
// quite nuanced. Good luck and happy hunting.
// NOTE: Don't exceed kMaxTimeoutMs. Typically 130ms.
// Suits most messages, while not swallowing many repeats.
const uint8_t kTimeout = 15;
// Alternatives:
// const uint8_t kTimeout = 90;
// Suits messages with big gaps like XMP-1 & some aircon units, but can
// accidentally swallow repeated messages in the rawData[] output.
//
// const uint8_t kTimeout = kMaxTimeoutMs;
// This will set it to our currently allowed maximum.
// Values this high are problematic because it is roughly the typical boundary
// where most messages repeat.
// e.g. It will stop decoding a message and start sending it to serial at
//      precisely the time when the next message is likely to be transmitted,
//      and may miss it.

// Set the smallest sized "UNKNOWN" message packets we actually care about.
// This value helps reduce the false-positive detection rate of IR background
// noise as real messages. The chances of background IR noise getting detected
// as a message increases with the length of the kTimeout value. (See above)
// The downside of setting this message too large is you can miss some valid
// short messages for protocols that this library doesn't yet decode.
//
// Set higher if you get lots of random short UNKNOWN messages when nothing
// should be sending a message.
// Set lower if you are sure your setup is working, but it doesn't see messages
// from your device. (e.g. Other IR remotes work.)
// NOTE: Set this value very high to effectively turn off UNKNOWN detection.
const uint16_t kMinUnknownSize = 12;

// How much percentage lee way do we give to incoming signals in order to match
// it?
// e.g. +/- 25% (default) to an expected value of 500 would mean matching a
//      value between 375 & 625 inclusive.
// Note: Default is 25(%). Going to a value >= 50(%) will cause some protocols
//       to no longer match correctly. In normal situations you probably do not
//       need to adjust this value. Typically that's when the library detects
//       your remote's message some of the time, but not all of the time.
const uint8_t kTolerancePercentage = kTolerance;  // kTolerance is normally 25%
// ==================== end of TUNEABLE PARAMETERS ====================

extern decode_results results;  // Storing the results

extern Volume volume;
extern MCP42010 frontVolume;
extern MCP42010 surroundVolume;
extern MCP42010 subCentVolume;


inline void setAllVolumeLevel()
{
    frontVolume.setWiper(MCP42010::WriteDeviceSelect::BothCh, volume.getVolume());
    surroundVolume.setWiper(MCP42010::WriteDeviceSelect::BothCh, volume.getSurroundVolume());
    subCentVolume.setWiper(MCP42010::WriteDeviceSelect::Ch0, volume.getCenterVolume());
    subCentVolume.setWiper(MCP42010::WriteDeviceSelect::Ch1, volume.getSubVolume());
}

#endif // MAIN_H