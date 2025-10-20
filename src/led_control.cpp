#include "led_control.h"

void LEDCONTROL::begin(int input, int output, int pwmCh, int pwmFreq, int pwmRes)
{
  inputPin = input;
  outputPin = output;
  pwmChannel = pwmCh;
  pwmFrequency = pwmFreq;
  pwmResolution = pwmRes;

  bool ok = ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
  if (!ok)
  {
    Serial.println("Gagal setup LEDC!");
  }

  ledcAttachPin(output, pwmChannel);
}

void LEDCONTROL::run()
{
  int adcValue = analogRead(inputPin);
  int duty = map(adcValue, 0, 4095, 0, 255);

  ledcWrite(pwmChannel, duty);

  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print(" | Duty: ");
  Serial.println(duty);
}

LEDCONTROL LEDC;