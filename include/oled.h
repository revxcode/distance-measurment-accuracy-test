#pragma once
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include "globals.h"
#include "jsnsr04t.h"
#include "ds18b20.h"
#include "globals.h"

static String colon(String text);

class OLED_SSD1306
{
public:
  void begin();
  void boot(const char *text = "DMAT");
  void error(const char *text = "error");
  void show(DateTime now);

private:
  void dateTime(DateTime now);
  void address();
  void distance();
  void temperature();
};

extern OLED_SSD1306 OLED;