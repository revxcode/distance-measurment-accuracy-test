#include "ds18b20.h"

TEMP_DS18B20::TEMP_DS18B20() : inputPin(0), oneWire(0), sensors(&oneWire) {}

void TEMP_DS18B20::begin(int inputSignal)
{
  inputPin = inputSignal;

  oneWire = OneWire(inputPin);
  sensors.begin();

  if (!sensors.getDeviceCount())
  {
    Serial.println("Error: No DS18B20 sensor found!");
    while (true)
    {
    }
  }

  Serial.println("DS18B20 initialized.");
  OLED.boot("DS18B20");
}

float TEMP_DS18B20::getTemperature()
{
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

TEMP_DS18B20 TEMP;
