#include <rtc.h>

RTC_DS3231 rtc;

void RTC3231::begin()
{
  OLED.boot("RTC3231");
  // Inisialisasi modul RTC, hentikan jika tidak terdeteksi
  if (!rtc.begin())
  {
    Serial.println("RTC not found!");
    OLED.error("RTC not found!");
    while (true)
    {
    }
  }

  Serial.println("RTC Initialized.");

  // Sinkronkan RTC dengan waktu kompilasi (sedikit offset)
  // rtc.adjust(DateTime(__DATE__, __TIME__) + TimeSpan(0, 0, 0, 10));
}

DateTime RTC3231::now()
{
  return rtc.now();
}

void RTC3231::setTime(DateTime dateTime)
{
  rtc.adjust(DateTime(dateTime));
}

RTC3231 RTC31;