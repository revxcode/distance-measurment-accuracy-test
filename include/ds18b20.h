#pragma once
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "oled.h"

class TEMP_DS18B20
{
private:
  int inputPin;
  OneWire oneWire;
  DallasTemperature sensors;

public:
  TEMP_DS18B20();
  void begin(int inputSignal);
  float getTemperature();
};

extern TEMP_DS18B20 TEMP;
