#include "jsnsr04t.h"

void JSNSR04T::begin(int trig, int echo)
{
  trigPin = trig;
  echoPin = echo;

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  OLED.boot("JSN-SR04T");
}

float JSNSR04T::getDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(50);
  digitalWrite(trigPin, HIGH);

  // Baca durasi echo
  long echoDurationUs = pulseIn(echoPin, HIGH, 30000UL); // timeout 30ms

  digitalWrite(trigPin, LOW);

  // jika jarak tidak ada
  if (echoDurationUs == 0)
  {
    Serial.println("NO ECHO");
    delay(500);
    return -1.0;
  }

  // Kecepatan suara pada 20°C
  const float speedOfSound = 343.0;                            // m/s
  const float cmPerMicrosecond = (speedOfSound * 100.0) / 1e6; // konversi ke cm/µs

  // Jarak = (waktu * kecepatan) / 2
  float distanceCm = (echoDurationUs * cmPerMicrosecond) / 2.0;

  return distanceCm;
}

JSNSR04T JSN;
