#include <Arduino.h>
#include "globals.h"
#include "oled.h"
#include "rtc.h"
#include "jsnsr04t.h"
#include "ds18b20.h"
#include "sdcard.h"
#include "web_server.h"

unsigned long lastUpdate = 0;
float dstLog;
float tempLog;

void setup()
{
  // Initialize serial
  Serial.begin(115200);

  OLED.begin();

  WS.setupLittleFS();
  WS.setupWiFiAP();
  WS.setupWebServer();

  RTC31.begin();
  JSN.begin(16, 17);
  TEMP.begin(27);
  SD2.begin();
}

void loop()
{

  // Handle incoming HTTP clients (non-blocking)
  WS.serverInit.handleClient();

  // Periodic measurement and logging (~1 Hz)
  unsigned long nowMillis = millis();

  if (nowMillis - lastUpdate >= 1000)
  {
    DateTime now = RTC31.now();
    // read sensors
    dstLog = JSN.getDistance();
    tempLog = TEMP.getTemperature();

    // update display
    OLED.show(now);

    // format CSV line and append to SD
    char data[32];
    snprintf(data, sizeof(data), "%04d-%02d-%02dT%02d:%02d:%02d,%.2f,%.2f",
             now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(), tempLog, dstLog);
    SD2.writeLog(data);

    lastUpdate = nowMillis;
  }

  // small sleep to reduce CPU usage
  delay(500);
}
