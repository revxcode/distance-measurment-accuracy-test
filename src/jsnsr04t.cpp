#include "jsnsr04t.h"

// Inisialisasi pin untuk sensor ultrasonic JSN-SR04T
void JSNSR04T::begin(int trig, int echo)
{
  // Tampilkan pesan boot pada OLED
  OLED.boot("JSN-SR04T");

  trigPin = trig;
  echoPin = echo;

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// Mengukur jarak dalam cm. Mengembalikan -1.0 jika tidak ada echo.
float JSNSR04T::getDistance()
{
  // Kirim sinyal trigger singkat
  digitalWrite(trigPin, LOW);
  delayMicroseconds(50);
  digitalWrite(trigPin, HIGH);

  // Baca durasi echo (timeout 30ms)
  long echoDurationUs = pulseIn(echoPin, HIGH, 30000UL);
  digitalWrite(trigPin, LOW);

  // Jika tidak ada echo
  if (echoDurationUs == 0)
  {
    Serial.println("NO ECHO");
    delay(500);
    return -1.0;
  }

  // Konversi waktu ke jarak (cm): (waktu_us * cm/us) / 2
  const float speedOfSound = 343.0;                            // m/s
  const float cmPerMicrosecond = (speedOfSound * 100.0) / 1e6; // cm per microsecond
  float distanceCm = (echoDurationUs * cmPerMicrosecond) / 2.0;

  return distanceCm;
}

JSNSR04T JSN;
