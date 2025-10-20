#include <rtc.h>

RTC_DS3231 rtc;

void RTC3231::begin()
{
  if (!rtc.begin())
  {
    Serial.println("RTC not found!");
    while (true)
    {
    }
  }

  Serial.println("RTC Initialized.");

  rtc.adjust(DateTime(__DATE__, __TIME__) + TimeSpan(0, 0, 0, 10));
  OLED.boot("RTC3231");
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