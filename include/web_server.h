#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "sdcard.h"
#include "rtc.h"

class WEB_SERVER
{
private:
  void sendWithCORS(int code, const String &type, const String &body);
  void handleRoot();
  void handleCSS();
  void handleJS();
  void handleListDir();
  void handleReadFile();
  void handleRtcSetTime();
  void handleFileManager();

public:
  WEB_SERVER();
  WebServer serverInit;
  void setupLittleFS();
  void setupWiFiAP();
  void setupWebServer();
};

extern WEB_SERVER WS;