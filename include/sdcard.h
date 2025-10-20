#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include "rtc.h"
#include "oled.h"

class SDCARD2
{
private:
  int csPinInit;
  char fileName[32];
  const char *getFileName(const char *prefix = "LOG_");

public:
  void begin(int csPin = 5);
  void writeLog(const char text[]);
  String listDir(const char *dirname, uint8_t levels);
  String readFile(const char *path);
};

extern SDCARD2 SD2;