#pragma once
#include <Arduino.h>
#include <RTClib.h>
#include "oled.h"

class RTC3231
{
public:
  void begin();
  DateTime now();
  void setTime(DateTime dateTime);

private:
};

extern RTC3231 RTC31;