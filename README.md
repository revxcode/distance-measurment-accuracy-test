# Uji Akurasi Pengukuran Jarak (Distance Measurement Accuracy Test)

Firmware for an ESP32-based data logger that measures distance (JSN-SR04T ultrasonic) and temperature (DS18B20), displays status on an SSD1306 OLED, logs CSV files to an SD card, and exposes a small web UI + API via LittleFS.

> Note: This README documents the repository as-is. No source code is modified.

## Key features

- Periodic measurements of temperature and distance (logged every second).
- Local CSV logging to SD card under `/data/log/dmat/` (daily files, prefixed `LOG_YYYYMMDD.csv`).
- Simple web UI served from LittleFS with a file manager and request endpoints to list/read logs.
- RTC (DS3231) integration for accurate timestamps and a web API to set the clock.
- SSD1306 OLED for boot messages and live status.

## Hardware (typical / default pins)

The firmware targets an ESP32. Default pins and peripherals are taken from the code in `src/main.cpp` and headers in `include/`.

- JSN-SR04T (ultrasonic)
  - `TRIG` -> GPIO 16
  - `ECHO` -> GPIO 17
  (Configured via `JSN.begin(16, 17)` in `src/main.cpp`)

- DS18B20 (OneWire temperature)
  - `DATA` -> GPIO 27
  - Use a 4.7k pull-up resistor from DATA to 3.3V
  (Configured via `TEMP.begin(27)`)

- SD Card (SPI)
  - `CS` (chip select) -> GPIO 5 (default in `SD2.begin()` — configured in `include/sdcard.h`/`src/sdcard.cpp`)
  - SPI signals use ESP32 VSPI default pins (typically MOSI=23, MISO=19, SCK=18) unless you changed wiring in hardware.

- OLED SSD1306 (I2C)
  - `SDA` -> SDA pin (ESP32 usually GPIO21)
  - `SCL` -> SCL pin (ESP32 usually GPIO22)
  - Shares I2C with DS3231 RTC

- DS3231 RTC (I2C)
  - `SDA` -> same as OLED SDA
  - `SCL` -> same as OLED SCL

Notes:
- Double-check your board pinout for your ESP32 variant. The code uses GPIO numbers; change wiring or code to match your board.

## Software / Dependencies

This project uses PlatformIO (recommended). The firmware depends on the following Arduino-compatible libraries (these names reflect common PlatformIO library names):

- OneWire
- DallasTemperature
- Adafruit GFX
- Adafruit SSD1306
- RTClib (for DS3231)
- LittleFS_esp32 (LittleFS implementation for ESP32)
- SD (built-in or `SD` library supporting your board)
- WiFi (ESP32 core)

PlatformIO will normally fetch these automatically if listed in `platformio.ini` or `lib_deps`. If you see compilation errors, install the missing libraries via PlatformIO or the Library Manager.

## How it works (quick)

- On boot, the device mounts LittleFS, attempts to join a configured WiFi network (STA) and falls back to AP mode if connecting fails.
- Sensors are initialised (ultrasonic + DS18B20 + RTC + OLED + SD).
- Every second (approx) the firmware reads temperature and distance, displays the values on the OLED, formats a timestamp using the RTC and appends a CSV line to the daily log file on the SD card.
- The web server (port 80) serves `index.html` and `file-manager.html` from LittleFS and provides small JSON/text endpoints to list and read log files.

## Web server defaults & API

Default WiFi credentials (set in `src/web_server.cpp`):

- Station (STA) attempt:
  - SSID: `Yansya Laundry II`
  - PASS: `qwerty123`
- Access Point (AP) fallback:
  - SSID: `ESP32-Device`
  - PASS: `12345678`

Endpoints (all served by the onboard web server):

- `/` — serves `index.html` from LittleFS
- `/file-manager` — serves `file-manager.html` from LittleFS
- `/style.css` and `/script.js` — static assets from LittleFS

API endpoints:

- GET `/api/listdir?path=/&levels=1`
  - Lists files/directories on the SD card. `path` defaults to `/`. `levels` controls recursion depth.

- GET `/api/readfile?path=/data/log/dmat/LOG_YYYYMMDD.csv`
  - Returns the text content of the file. Provide exact path as `path` parameter.

- GET `/api/rtc/set-time?yyyy=YYYY&mm=MM&dd=DD&h=HH&m=MM&s=SS`
  - Sets the RTC time (validates values). Example:

```bash
# set to 2025-10-21 12:34:56
curl "http://<device-ip>/api/rtc/set-time?yyyy=2025&mm=10&dd=21&h=12&m=34&s=56"
```

Notes:
- The device first tries to join the STA network and prints the local IP on success. If it can't connect within ~8 seconds, it starts a WiFi AP. Check the serial console at 115200 baud for status messages and the AP IP.
- LittleFS files that the web UI expects are present in the `data/` folder of this repository (`index.html`, `file-manager.html`, `script.js`, `style.css`). Ensure these are uploaded to LittleFS before use.

## Log files

- Logs are stored under `/data/log/dmat/` on the SD card.
- File names: `LOG_YYYYMMDD.csv` (prefix `LOG_` defined in `src/sdcard.cpp`).
- Each new day creates a new CSV. The first time a file is created, a header line `timestamp, temperature, distance` is written.
- CSV line format (example):
  - `2025-10-21T12:34:56,23.45,120.50`
  - Fields: ISO timestamp (from RTC), temperature (°C), distance (units from ultrasonic sensor — see calibration),

## Build & upload (PlatformIO)

From the project root (this repository contains a `platformio.ini`):

```bash
# build firmware
pio run

# upload firmware to the default environment (set your board in platformio.ini)
pio run -t upload

# monitor serial at 115200
pio device monitor -b 115200
```

LittleFS contents (the `data/` folder) must be uploaded to the device filesystem if you want the onboard web UI available. If your `platformio.ini` is configured with the `uploadfs` target and LittleFS plugin, you can upload filesystem contents with:

```bash
pio run -t uploadfs
```

If `uploadfs` isn't configured, consult the PlatformIO / LittleFS docs for how to upload the `data/` folder to your board's LittleFS partition.

## Where to change settings

- WiFi credentials and AP fallback: `src/web_server.cpp` (variables `STA_SSID`, `STA_PASS`, `AP_SSID`, `AP_PASS`).
- Sensor pins and SD CS: defined at init calls in `src/main.cpp` and default params in headers (`include/*`).
- CSV prefix is controlled in `src/sdcard.cpp` (function `getFileName()` — default prefix is `LOG_`).

## Troubleshooting

- SD card mount fails and device hangs on boot:
  - Message printed: `SD Card mount failed!` — firmware enters an infinite loop.
  - Check the CS pin wiring (default GPIO5) and SD card power/voltage (3.3V for ESP32).
  - Try a different SD card or format it FAT32.

- LittleFS not mounting:
  - Check serial logs: `LittleFS Mount Failed even after format!`. Consider reformatting or re-uploading the LittleFS partition.

- No WiFi connection / stuck in STA:
  - Check `STA_SSID` and `STA_PASS` in `src/web_server.cpp`.
  - After failing to connect, the device starts an AP (`ESP32-Device`) — connect to it and use the printed AP IP.

- RTC shows wrong time:
  - Use the web API `/api/rtc/set-time` to set the time, or set time in code via `RTC31.setTime(...)`.

- No readings from sensors:
  - For DS18B20: confirm 1-Wire wiring and pull-up resistor (4.7k). Try replacing the sensor or check `TEMP.getTemperature()` serial output.
  - For JSN-SR04T: check TRIG/ECHO wiring and ensure it's powered correctly. Some JSN modules are higher-voltage tolerant; use voltage-compatible wiring.

## Project layout

- `include/` - header files used by the firmware
- `src/` - source files (firmware implementation)
- `data/` - static web UI files to be uploaded to LittleFS (index.html, file-manager.html, etc.)
- `web/` - additional web assets
- `platformio.ini` - PlatformIO configuration
- `package.json` - (present in repo; may be used for local web assets or tooling)

## Safety & notes

- This firmware writes to the SD card frequently. Use a high-quality SD card and avoid power removal during writes to prevent corruption.
- The default WiFi credentials in source are example values — change them before deploying in an untrusted network.

## License

This repository contains code and assets with no explicit license file. If you plan to reuse or redistribute, add an appropriate LICENSE file or contact the original author.

---

If you want, I can:

- Add a small `README` section showing how to upload the LittleFS contents for this particular `platformio.ini` if you share that file contents or confirm the environment.
- Add an example script to download logs over the API or parse CSVs locally.

