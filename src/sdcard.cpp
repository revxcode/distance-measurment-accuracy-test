#include "sdcard.h"

// Modul SDCARD2: lokasi penyimpanan dan utilitas untuk menulis/menbaca log CSV
static const char *filePath = "/data/log/dmat/";

void SDCARD2::begin(int csPin)
{
  csPinInit = csPin;

  // Inisialisasi SD card. Jika gagal, tampilkan pesan dan hentikan.
  if (!SD.begin(csPinInit))
  {
    Serial.println("SD Card mount failed!");
    while (true)
    {
    }
  }

  // Pastikan struktur direktori untuk log ada, buat jika perlu
  if (!SD.exists(filePath))
  {
    Serial.println("/data/log/dmat not found.");
    SD.mkdir("/data");
    SD.mkdir("/data/log");
    SD.mkdir(filePath);
    Serial.println("Created directories: /data/log/dmat");
  }

  Serial.println("SD Card initialized.");
  Serial.printf("Card size: %llu MB\n", SD.cardSize() / (1024 * 1024));
  OLED.boot("SDCARD");
}

const char *SDCARD2::getFileName(const char *prefix)
{
  DateTime now = RTC31.now();

  // Buat nama file harian: /<prefix>YYYYMMDD.csv
  snprintf(fileName, sizeof(fileName), "/%s%04d%02d%02d.csv", prefix, now.year(), now.month(), now.day());
  return fileName;
}

void SDCARD2::writeLog(const char text[])
{
  // Pastikan SD masih ter-mount; coba remount satu kali jika perlu
  if (!SD.begin(csPinInit))
  {
    Serial.println("SD not initialized, remounting...");
    if (!SD.begin(csPinInit))
    {
      Serial.println("Failed to reinitialize SD!");
      return;
    }
  }

  char fullPath[64];
  snprintf(fullPath, sizeof(fullPath), "%s%s", filePath, getFileName());

  if (fullPath[0] != '/')
  {
    char temp[64];
    snprintf(temp, sizeof(temp), "/%s", fullPath);
    strncpy(fullPath, temp, sizeof(fullPath));
    fullPath[sizeof(fullPath) - 1] = '\0';
  }

  File fileLog;

  if (!SD.exists(fullPath))
  {
    // Jika file belum ada: buat file, tulis header, lalu baris pertama
    Serial.printf("File %s doesn't exist, creating new file...\n", fullPath);

    fileLog = SD.open(fullPath, "w");
    if (fileLog)
    {
      Serial.printf("%s created.\n", getFileName());
      Serial.println(text);
      fileLog.println("timestamp, temperature, distance");
      fileLog.println(text);
      fileLog.close();
    }
    else
    {
      Serial.printf("%s Fail to open.\n", getFileName());
    }
  }
  else
  {
    // Jika file sudah ada: buka dalam mode append dan tambahkan baris
    fileLog = SD.open(fullPath, "a");
    if (fileLog)
    {
      Serial.println(text);
      fileLog.println(text);
      fileLog.close();
    }
  }
}

String SDCARD2::listDir(const char *dirname, uint8_t levels)
{
  Serial.println(dirname);
  Serial.println(levels);

  String result = "";
  File root = SD.open(dirname);
  if (!root)
  {
    result = "Failed to open directory\n";
    return result;
  }
  if (!root.isDirectory())
  {
    result = "Not a directory\n";
    root.close();
    return result;
  }

  File file = root.openNextFile();
  while (file)
  {
    result += file.name();
    if (file.isDirectory())
    {
      result += "/\n";
      if (levels)
      {
        result += listDir(file.name(), levels - 1);
      }
    }
    else
    {
      result += "\n";
    }
    file = root.openNextFile();
  }
  root.close();
  return result;
}

String SDCARD2::readFile(const char *path)
{
  // Baca seluruh konten file dan kembalikan sebagai String
  String content = "";
  File file = SD.open(path);
  if (!file)
  {
    content = "Failed to open file\n";
    return content;
  }
  while (file.available())
  {
    content += (char)file.read();
  }
  file.close();
  return content;
}

SDCARD2 SD2;