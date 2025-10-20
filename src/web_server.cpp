#include "web_server.h"

WEB_SERVER::WEB_SERVER() : serverInit(80) {}

const char *STA_SSID = "SSID";
const char *STA_PASS = "password";

const char *AP_SSID = "ESP32-Device";
const char *AP_PASS = "12345678";

void WEB_SERVER::setupWiFiAP()
{
  Serial.println("\n[WiFi] Connecting to STA...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_SSID, STA_PASS);

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 8000)
  {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("\nFailed to connect, starting AP mode...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);
    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP Mode IP Address: ");
    Serial.println(apIP);
  }
}

void WEB_SERVER::sendWithCORS(int code, const String &type, const String &body)
{
  serverInit.sendHeader("Access-Control-Allow-Origin", "*");
  serverInit.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  serverInit.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  serverInit.send(code, type, body);
}

void WEB_SERVER::handleRoot()
{
  File file = LittleFS.open("/index.html", "r");
  if (!file)
  {
    sendWithCORS(404, "text/plain", "File Not Found");
    return;
  }
  serverInit.streamFile(file, "text/html");
  file.close();
}

void WEB_SERVER::handleFileManager()
{
  File file = LittleFS.open("/file-manager.html", "r");
  if (!file)
  {
    sendWithCORS(404, "text/plain", "File Not Found");
    return;
  }
  serverInit.streamFile(file, "text/html");
  file.close();
}

void WEB_SERVER::handleCSS()
{
  File file = LittleFS.open("/style.css", "r");
  if (!file)
  {
    sendWithCORS(404, "text/plain", "File Not Found");
    return;
  }
  serverInit.streamFile(file, "text/css");
  file.close();
}

void WEB_SERVER::handleJS()
{
  File file = LittleFS.open("/script.js", "r");
  if (!file)
  {
    sendWithCORS(404, "text/plain", "File Not Found");
    return;
  }
  serverInit.streamFile(file, "application/javascript");
  file.close();
}

void WEB_SERVER::setupLittleFS()
{
  if (!LittleFS.begin(true))
  {
    Serial.println("LittleFS Mount Failed even after format!");
  }
  else
  {
    Serial.println("LittleFS mounted successfully!");
  }
}

void WEB_SERVER::setupWebServer()
{
  serverInit.on("/", HTTP_GET, [this]()
                { this->handleRoot(); });
  serverInit.on("/file-manager", HTTP_GET, [this]()
                { this->handleFileManager(); });
  serverInit.on("/style.css", HTTP_GET, [this]()
                { this->handleCSS(); });
  serverInit.on("/script.js", HTTP_GET, [this]()
                { this->handleJS(); });
  serverInit.on("/api/listdir", HTTP_GET, [this]()
                { this->handleListDir(); });
  serverInit.on("/api/readfile", HTTP_GET, [this]()
                { this->handleReadFile(); });
  serverInit.on("/api/rtc/set-time", HTTP_GET, [this]()
                { this->handleRtcSetTime(); });

  serverInit.begin();
  Serial.println("HTTP server started");
}

void WEB_SERVER::handleListDir()
{
  String path = serverInit.arg("path");
  if (path.isEmpty())
    path = "/";
  uint8_t levels = serverInit.hasArg("levels") ? serverInit.arg("levels").toInt() : 1;
  String result = SD2.listDir(path.c_str(), levels);
  sendWithCORS(200, "application/json", result);
}

void WEB_SERVER::handleReadFile()
{
  String path = serverInit.arg("path");
  if (path.isEmpty())
  {
    sendWithCORS(400, "text/plain", "Missing 'path' parameter");
    return;
  }
  String result = SD2.readFile(path.c_str());
  if (result.isEmpty())
  {
    sendWithCORS(404, "text/plain", "File Not Found");
    return;
  }
  sendWithCORS(200, "text/plain", result);
}

void WEB_SERVER::handleRtcSetTime()
{
  int yyyy = serverInit.arg("yyyy").toInt();
  int mm = serverInit.arg("mm").toInt();
  int dd = serverInit.arg("dd").toInt();
  int h = serverInit.arg("h").toInt();
  int m = serverInit.arg("m").toInt();
  int s = serverInit.arg("s").toInt();

  if (yyyy < 2000 || mm < 1 || mm > 12 || dd < 1 || dd > 31 || h < 0 || h > 23 || m < 0 || m > 59 || s < 0 || s > 59)
  {
    sendWithCORS(400, "text/plain", "Invalid date/time parameters");
    return;
  }

  DateTime dt(yyyy, mm, dd, h, m, s);
  RTC31.setTime(dt);

  sendWithCORS(200, "text/plain", "OK");
}

WEB_SERVER WS;