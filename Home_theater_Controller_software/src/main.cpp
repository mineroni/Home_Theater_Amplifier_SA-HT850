#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

#include "main.h"

#include "secrets.h"
#include "utils.hpp"
#include "IR/IR.hpp"
#include "VolumeController/Volume.h"
#include "Webserver/webServer.h"
#include "DigitalPotDriver/MCP42010.h"

unsigned long lastIRTime;
void handleIRCommand();
Volume volume;
decode_results results;  // Storing the results
// Create an instance of the web server on port 80
ESP8266WebServer server(80);
// Creating channel control potmeters
MCP42010 frontVolume(SPI_CS0, SPI_SCK, SPI_MOSI);
MCP42010 surroundVolume(SPI_CS1, SPI_SCK, SPI_MOSI);
MCP42010 subCentVolume(SPI_CS2, SPI_SCK, SPI_MOSI);

void setup() 
{
  // Initializing Serial port
  initSerial();
  Println("\nBooting...");

  // Initializing IR
  initIR();

  // Initializing pins
  pinMode(WiFiLed, OUTPUT);
  pinMode(IRLed, OUTPUT);
  pinMode(standbyButton, INPUT_PULLUP);
  digitalWrite(WiFiLed, LOW);
  digitalWrite(IRLed, LOW);

  // Setting up WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  digitalWrite(WiFiLed, HIGH); // ESP is connected to WiFi

  // Setting up OTA
  ArduinoOTA.begin();
  ArduinoOTA.setHostname("Home-Theater System");
  ArduinoOTA.setPassword("Ronald6.");

  // Setting up web server
  setupWebServer();

  Println("Ready");
  Print("IP address: ");
  Println(WiFi.localIP());

  // Waiting for the power lines to initialize
  while (millis() < 3000)
    yield();  

  // Setting initial volume level
  setAllVolumeLevel();

  lastIRTime = millis();
}

void loop() 
{
  handleIRCommand();

  server.handleClient();

  digitalWrite(WiFiLed, WiFi.status() == WL_CONNECTED ? HIGH : LOW);

  yield();
}

void handleIRCommand()
{
  if(millis() - lastIRTime < captureDelayIR)
  {
    handleIR();
    return;
  }

  digitalWrite(IRLed, LOW);

  switch (handleIR())
  {
    case IR_Mute:
      Println("Mute");
      lastIRTime = millis();
      volume.mute();
      volume.printVolumeStatus();
      setAllVolumeLevel();
      digitalWrite(IRLed, HIGH);
      break;

    case IR_VolumeUp:
      Println("Volume Up");
      lastIRTime = millis();
      volume.changeVolume(volumeChangeStep);
      volume.printVolumeStatus();
      setAllVolumeLevel();
      digitalWrite(IRLed, HIGH);
      break;

    case IR_VolumeDown:
      Println("Volume Down");
      lastIRTime = millis();
      volume.changeVolume(-volumeChangeStep);
      volume.printVolumeStatus();
      setAllVolumeLevel();
      digitalWrite(IRLed, HIGH);
      break;

    case IR_Source:
      Println("Source select");
      lastIRTime = millis();
      digitalWrite(IRLed, HIGH);
      break;

    case IR_UNKNOWN:
      Println("Unknown command:");
      Print(resultToHumanReadableBasic(&results));
      Print(resultToSourceCode(&results));
      digitalWrite(IRLed, HIGH);
      break;

    default: // Probably IR_NOOP
      break;
  }  
}