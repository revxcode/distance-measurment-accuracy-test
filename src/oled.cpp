#include "oled.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void OLED_SSD1306::begin()
{
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("SSD1306 not found!");
    while (true)
    {
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();

  Serial.println("OLED initialized.");
  // Tampilkan layar boot singkat
  boot("DMAT");
}

void OLED_SSD1306::boot(const char *text)
{
  const int textSize = 2;

  display.clearDisplay();

  display.setTextSize(textSize);
  display.setCursor((SCREEN_WIDTH - (strlen(text) * (6 * textSize))) / 2, 20);
  display.print(text);
  display.display();

  int barWidth = SCREEN_WIDTH - 40;
  int barX = 20;
  int barY = 45;
  display.drawRect(barX, barY, barWidth, 10, SSD1306_WHITE);
  display.display();

  for (int i = 0; i <= barWidth - 2; i += 6)
  {
    display.fillRect(barX + 1, barY + 1, i, 8, SSD1306_WHITE);
    display.display();
    if (text == "DMAT")
    {
      delay(500);
    }
  }

  display.clearDisplay();
  display.display();
}

// display date & time
void OLED_SSD1306::dateTime(DateTime now)
{
  // Format dan tampilkan tanggal serta waktu di kiri atas
  display.setTextSize(1);
  display.setCursor(0, 2);
  if (now.day() < 10)
    display.print(0);
  display.print(now.day());
  display.print('/');
  if (now.month() < 10)
    display.print(0);
  display.print(now.month());
  display.print('/');
  display.print(now.year());

  display.setTextSize(1);
  display.setCursor(80, 2);
  if (now.hour() < 10)
    display.print('0');
  display.print(now.hour());
  display.print(':');
  if (now.minute() < 10)
    display.print('0');
  display.print(now.minute());
  display.print(':');
  if (now.second() < 10)
    display.print('0');
  display.print(now.second());
}

// display distance
void OLED_SSD1306::distance()
{
  // Tampilkan label dan nilai jarak (cm)
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print(colon("DST"));
  display.print(dstLog);
  display.print(" cm");
}

// display temperature
void OLED_SSD1306::temperature()
{
  // Tampilkan label dan nilai suhu (°C)
  display.setTextSize(1);
  display.setCursor(0, 36);
  display.print(colon("TEMP"));
  display.print(tempLog);
  display.print(" ");
  display.print((char)247);
  display.print("C");
}

void OLED_SSD1306::show(DateTime now)
{
  display.clearDisplay();

  display.fillRect(0, 0, 128, 12, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  dateTime(now);

  display.setTextColor(SSD1306_WHITE);
  distance();
  temperature();

  display.display();
}

static String colon(String text)
{
  int textLength = text.length();
  int maxLength = 5;

  if (textLength > maxLength)
    return "Text too long";

  for (int i = 0; i < (maxLength - textLength); i++)
  {
    text = text + " ";
  }

  return text + ": ";
}

OLED_SSD1306 OLED;