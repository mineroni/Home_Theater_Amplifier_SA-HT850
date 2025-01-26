#include <ESP8266WebServer.h>

#include "webPageIndex.h"
#include "webPageAdvanced.h"
#include "Volume.h"
#include "utils.hpp"

extern Volume volume;
extern ESP8266WebServer server;


inline const char* generateResponseMessage() {
    static char buffer[256];
    snprintf(buffer, sizeof(buffer),
        "{"
        "\"volume\": %d, "
        "\"muted\": %s, "
        "\"surround_offset\": %d, "
        "\"subwoofer_offset\": %d, "
        "\"center_offset\": %d"
        "}",
        volume.getVolume(),
        volume.isMuted() ? "true" : "false",
        volume.getOffsets()[Volume::ChannelID::SURROUND],
        volume.getOffsets()[Volume::ChannelID::SUBWOOFER],
        volume.getOffsets()[Volume::ChannelID::CENTER]
    );
    return buffer;
}

// Function to set the volume level
void handleAddVolume() {
    if (server.hasArg("level")) {
        volume.changeVolume(server.arg("level").toInt());
    }
    volume.printVolumeStatus();
    server.send(200, "application/json", generateResponseMessage());
}

// Function to add the offset to the volume level of the Surround channel
void handleAddOffsetSurround() {
    if (server.hasArg("offset")) {
        volume.changeSurroundOffset(server.arg("offset").toInt());
    }
    volume.printVolumeStatus();
    server.send(200, "application/json", generateResponseMessage());
}

// Function to add the offset to the volume level of the Subwoofer channel
void handleAddOffsetSubwoofer() {
    if (server.hasArg("offset")) {
        volume.changeSubOffset(server.arg("offset").toInt());
    }
    volume.printVolumeStatus();
    server.send(200, "application/json", generateResponseMessage());
}

// Function to add the offset to the volume level of the Center channel
void handleAddOffsetCenter() {
    if (server.hasArg("offset")) {
        volume.changeCenterOffset(server.arg("offset").toInt());
    }
    volume.printVolumeStatus();
    server.send(200, "application/json", generateResponseMessage());
}

// Function to set the mute status
void handleMute() {
    volume.mute();
    volume.printVolumeStatus();
    server.send(200, "application/json", generateResponseMessage());
}

void setupWebServer() {
    // Define web routes
    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", indexPage);
    });
    server.on("/get-status", HTTP_GET, []() {
        server.send(200, "application/json", generateResponseMessage());
    });
    server.on("/advanced", HTTP_GET, []() {
        server.send(200, "text/html", advancedPage);
    });
    server.on("/reset-offsets", HTTP_GET, []() {
        volume.resetOffsets();
        server.send(200, "application/json", generateResponseMessage());
    });
    server.on("/add-volume", HTTP_POST, handleAddVolume);
    server.on("/add-offsetSurround", HTTP_POST, handleAddOffsetSurround);
    server.on("/add-offsetSubwoofer", HTTP_POST, handleAddOffsetSubwoofer);
    server.on("/add-offsetCenter", HTTP_POST, handleAddOffsetCenter);
    server.on("/mute", HTTP_POST, handleMute);
    server.begin();
}