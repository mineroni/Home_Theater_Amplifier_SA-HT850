#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

#include "main.h"

#include "secrets.h"
#include "utils.hpp"
#include "IR\IR.hpp"
#include "VolumeController\Volume.h"
#include "Webserver\webServer.h"
#include "DigitalPotDriver\MCP42010.h"

unsigned long lastIRTime;
void handleIRCommand();
Volume volume;
decode_results results;  // Storing the results
// Create an instance of the web server on port 80
ESP8266WebServer server(80);
// Creating channel control potmeters
MCP42010 frontVolume(CS0, SCK, MOSI);
MCP42010 surroundVolume(CS1, SCK, MOSI);
MCP42010 subCentVolume(CS2, SCK, MOSI);

void setup() 
{
  // Initializing Serial port
  initSerial();
  Println("\nBooting...");

  // Initializing IR
  initIR();

  // Setting up WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Println("Connection Failed! Rebooting...");
    delay(5000);
    return;
  }

  // Setting up OTA
  ArduinoOTA.begin();
  ArduinoOTA.setHostname("Home-Theater System");
  ArduinoOTA.setPassword("Ronald6.");

  // Setting up web server
  setupWebServer();

  Println("Ready");
  Print("IP address: ");
  Println(WiFi.localIP());

  // Setting initial volume level
  setAllVolumeLevel();

  lastIRTime = millis();
}

void loop() 
{
  handleIRCommand();

  server.handleClient();

  yield();
}

void handleIRCommand()
{
  if(millis() - lastIRTime < captureDelayIR)
  {
    handleIR();
    return;
  }

  switch (handleIR())
  {
    case IR_Mute:
      Println("Mute");
      lastIRTime = millis();
      volume.mute();
      volume.printVolumeStatus();
      setAllVolumeLevel();
      break;

    case IR_VolumeUp:
      Println("Volume Up");
      lastIRTime = millis();
      volume.changeVolume(volumeChangeStep);
      volume.printVolumeStatus();
      setAllVolumeLevel();
      break;

    case IR_VolumeDown:
      Println("Volume Down");
      lastIRTime = millis();
      volume.changeVolume(-volumeChangeStep);
      volume.printVolumeStatus();
      setAllVolumeLevel();
      break;

    case IR_Source:
      Println("Source select");
      lastIRTime = millis();
      break;

    case IR_UNKNOWN:
      Println("Unknown command:");
      Print(resultToHumanReadableBasic(&results));
      Print(resultToSourceCode(&results));
      break;

    default: // Probably IR_NOOP
      break;
  }  
}