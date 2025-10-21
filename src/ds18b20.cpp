#include "ds18b20.h"

// Konstruktor: inisialisasi kelas sensor
TEMP_DS18B20::TEMP_DS18B20() : inputPin(0), oneWire(0), sensors(&oneWire) {}

// Inisialisasi OneWire pada pin yang diberikan dan cek keberadaan sensor
void TEMP_DS18B20::begin(int inputSignal)
{
  OLED.boot("DS18B20");

  inputPin = inputSignal;
  oneWire = OneWire(inputPin);
  sensors.begin();

  // Jika tidak ada perangkat, hentikan untuk debugging
  if (!sensors.getDeviceCount())
  {
    Serial.println("DS18B20 not found!");
    OLED.error("DS18B20 not found!");
    while (true)
    {
    }
  }

  Serial.println("DS18B20 initialized.");
}

// Minta pembacaan suhu dan kembalikan nilai (°C)
float TEMP_DS18B20::getTemperature()
{
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

TEMP_DS18B20 TEMP;
