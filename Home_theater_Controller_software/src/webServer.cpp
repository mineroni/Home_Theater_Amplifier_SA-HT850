#include <ESP8266WebServer.h>

#include "webPages.h"
#include "Volume.h"
#include "utils.hpp"

extern Volume volume;
extern ESP8266WebServer server;

// Function to handle the web page request
void handleIndex() 
{
    server.send(200, "text/html", indexPage);
}

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
        volume.getSurroundVolume(),
        volume.getSubVolume(),
        volume.getCenterVolume()
    );
    return buffer;
}

// Function to get current status of the volume and mute state
void handleStatus() 
{
    server.send(200, "application/json", generateResponseMessage());
}

// Function to set the volume level
void handleAddVolume() {
    if (server.hasArg("level")) {
        volume.changeVolume(server.arg("level").toInt());
    }
    Println(server.args());
    volume.printVolumeStatus();
    server.send(200, "text/plain", generateResponseMessage());
}

// Function to set the mute status
void handleMute() {
    volume.mute();
    volume.printVolumeStatus();
    server.send(200, "text/plain", generateResponseMessage());
}

void setupWebServer() {
    // Define web routes
    server.on("/", HTTP_GET, handleIndex);
    server.on("/get-status", HTTP_GET, handleStatus);
    server.on("/add-volume", HTTP_POST, handleAddVolume);
    server.on("/mute", HTTP_POST, handleMute);
    server.begin();
}