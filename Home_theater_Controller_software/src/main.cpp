#include <Arduino.h>
#include <ArduinoOTA.h>

void setup() 
{
  Serial.begin(115200);
  Serial.println("Booting");

  WiFi.begin("SSID", "PASSWORD");

  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.begin();
  ArduinoOTA.setHostname("Home-Theater System");
  ArduinoOTA.setPassword("Ronald6.");
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{

}