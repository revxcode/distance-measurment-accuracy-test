#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>
#include "ds18b20.h"
#include "oled.h"

class JSNSR04T
{
public:
  void begin(int trig, int echo);
  float getDistance();

private:
  int trigPin;
  int echoPin;
};

extern JSNSR04T JSN;
