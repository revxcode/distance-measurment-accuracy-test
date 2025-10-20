#pragma once

#include <Arduino.h>

class LEDCONTROL
{
private:
  int inputPin;
  int outputPin;
  int pwmChannel;
  int pwmFrequency;
  int pwmResolution;

public:
  void begin(
      int input = 15,
      int output = 12,
      int pwmCh = 0,
      int pwmFreq = 1000,
      int pwmRes = 8);

  void run();
};

extern LEDCONTROL LEDC;