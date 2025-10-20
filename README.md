# Distance Measurement Accuracy Test — ESP32 logger

![thumbnail](thumbnail.webp)

What it does
- Logs temperature (DS18B20) and distance (JSN-SR04T) to CSV files on an SD card.
- Shows status on SSD1306 OLED and uses a DS3231 RTC for timestamps.
- Serves a minimal web UI from LittleFS and provides simple APIs to list/read logs and set the RTC.

Quick specs
- Platform: ESP32 (PlatformIO)
- Logs: `/data/log/dmat/LOG_YYYYMMDD.csv` (CSV header: `timestamp, temperature, distance`)
- Sampling: ~1 Hz (as implemented in `src/main.cpp`)

Default pins (check your board):
- Ultrasonic (JSN-SR04T): TRIG=GPIO16, ECHO=GPIO17
- DS18B20 (1-Wire): DATA=GPIO27 (use 4.7k pull-up)
- SD CS: GPIO5 (SPI uses standard VSPI pins)
- I2C (OLED + RTC): SDA/SCL (common ESP32 defaults SDA=21, SCL=22)

Web & networking
- Tries STA first (credentials in `src/web_server.cpp`), falls back to AP mode.
- Default STA: `SSID` / `password`
- Default AP: `ESP32-Device` / `12345678`

Useful endpoints
- GET `/api/listdir?path=/&levels=1` — list files on SD
- GET `/api/readfile?path=/data/log/dmat/LOG_YYYYMMDD.csv` — get file contents
- GET `/api/rtc/set-time?yyyy=YYYY&mm=MM&dd=DD&h=HH&m=MM&s=SS` — set RTC

Build & upload (PlatformIO)
From project root:

```bash
# build
pio run

# upload firmware
pio run -t upload

# upload LittleFS (if configured)
pio run -t uploadfs

# serial monitor
pio device monitor -b 115200
```

Notes & troubleshooting
- SD: if `SD Card mount failed!` appears the device enters a loop — check CS wiring (GPIO5) and 3.3V power.
- LittleFS: if mount fails, reformat/upload the `data/` folder.
- WiFi: check STA credentials; device prints IP or AP state on serial at 115200.
- Sensors: verify wiring and pull-ups (DS18B20). Check serial logs for readings.

Where to change things
- WiFi creds: `src/web_server.cpp`
- Pins & sensor init: `src/main.cpp`
- CSV filename prefix: `src/sdcard.cpp`

License
- No license file included. Add a LICENSE if you plan to redistribute.

Want it shorter, translated, or with sample scripts to fetch logs? Tell me which and I’ll update.

