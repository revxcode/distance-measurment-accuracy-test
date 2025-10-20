#include <Arduino.h>
#include <utils.h>

void ledBlinking(int pin, unsigned long delayMs, int repeat)
{
  for (int i = 0; i < repeat; i++)
  {
    digitalWrite(pin, HIGH);
    delay(delayMs);
    digitalWrite(pin, LOW);
    delay(delayMs);
  }
}