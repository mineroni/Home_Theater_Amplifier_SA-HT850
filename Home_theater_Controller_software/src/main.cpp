#include <Arduino.h>
#include <ArduinoOTA.h>

#include "main.h"

#include "secrets.h"
#include "utils.hpp"
#include "IR.hpp"
#include "Volume.h"

unsigned long lastIRTime;
void handleIRCommand();
Volume volume;
bool changeVolumeNeeded;
decode_results results;  // Storing the results

void setup() 
{
  // Initializing Serial port
  initSerial();
  Println("\nBooting...");

  // TODO: Setting volume to 0

  // Initializing IR
  initIR();

  // Setting up WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Println("Connection Failed! Rebooting...");
    delay(5000);
    return;
    //ESP.restart();
  }

  // Setting up OTA
  ArduinoOTA.begin();
  ArduinoOTA.setHostname("Home-Theater System");
  ArduinoOTA.setPassword("Ronald6.");
  Println("Ready");
  Print("IP address: ");
  Println(WiFi.localIP());

  // TODO: Setting volume level as needed

  lastIRTime = millis();
  changeVolumeNeeded = false;
}

void loop() 
{
  handleIRCommand();

  if(changeVolumeNeeded)
  {
    changeVolumeNeeded = false;
    // TODO: Setting changed volume
  }

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
      changeVolumeNeeded = true;
      volume.mute();
      volume.printVolumeStatus();
      break;

    case IR_VolumeUp:
      Println("Volume Up");
      lastIRTime = millis();
      changeVolumeNeeded = true;
      volume.changeVolume(volumeChangeStep);
      volume.printVolumeStatus();
      break;

    case IR_VolumeDown:
      Println("Volume Down");
      lastIRTime = millis();
      changeVolumeNeeded = true;
      volume.changeVolume(-volumeChangeStep);
      volume.printVolumeStatus();
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

    default:
      break;
  }  
}