
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "ArduinoJson.h"
#include "HeaterOperator.h"
#include "ArduinoOTA.h"

AsyncWebServer server(80);
HeaterOperator heaterOperator(14, 0);

const char *ssid = "Sonoma #3 Google Wifi";
const char *password = "word_dawg1999";

const char *PARAM_MESSAGE = "message";

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "application/json", "{\"errorMessage\":\"Page not found\"}");
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  Serial.printf("On Body!\n");
  // Serial.printf("[REQUEST]\t%s\r\n", (const char*)data);
  request->send(200, "text/plain", "JSON body requested");
}

void setup()
{

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  configTime(-28800, 0, "north-america.pool.ntp.org");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      Serial.println("Auth Failed");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      Serial.println("Begin Failed");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      Serial.println("Connect Failed");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      Serial.println("Receive Failed");
    }
    else if (error == OTA_END_ERROR)
    {
      Serial.println("End Failed");
    }
  });

  ArduinoOTA.begin();


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String page = "<html><head></head><body>"
                  "<div>Temp: #temp</div>	<div>Time: #time</div>	"
                  "</body</html>";

    struct tm timeinfo;
    char timeStringBuff[50] = "00/00/0000";
    if (getLocalTime(&timeinfo))
    {
      strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    }

    page.replace("#temp", String((temperatureRead() - 36) / 1.8));
    page.replace("#time", String(timeStringBuff));
    request->send(200, "text/html", page);
  });

  // Send a GET request to <IP>/get?message=<message>
  server.on("/getTime", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Get requested: ");
    struct tm timeinfo;
    try
    {
      if (getLocalTime(&timeinfo))
      {
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      }
    }
    catch (std::exception err)
    {
      Serial.println(err.what());
    }

    String message;
    if (request->hasParam(PARAM_MESSAGE))
    {
      message = request->getParam(PARAM_MESSAGE)->value();
    }
    else
    {
      message = "No message sent";
    }
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    request->send(200, "text/plain", "Hello, Local Time: " + String(timeStringBuff) + " Temp: " + String((temperatureRead() - 36) / 1.8));
  });

  // Send a POST request to <IP>/post with a form field message set to <message>
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("Post requested: ");
    String message;

    if (request->hasParam(PARAM_MESSAGE, true))
    {
      message = request->getParam(PARAM_MESSAGE, true)->value();
      message = "Has params";
    }
    else
    {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, POST: " + message);
  });

  server.on("/manage", HTTP_POST, [](AsyncWebServerRequest *request) {},
            NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      Serial.println("JSON manage accepted");

      heaterOperator.ProcessCommand(data);
      request->send(200, "text/plain", "JSON received :) Temp: " + String((temperatureRead()-36)/1.8)); });
  server.on("/config", HTTP_POST, [](AsyncWebServerRequest *request) {},
            NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      Serial.println("JSON config accepted");

      heaterOperator.SetConfig(data);
      request->send(200, "text/plain", "Config saved :) Temp: " + String((temperatureRead()-36)/1.8)); });

  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("JSON config requested.  Temp: " + String((temperatureRead() - 36) / 1.8));
    try
    {
      char jsonConfig[2000];
      heaterOperator.GetConfig(jsonConfig);
      Serial.println(jsonConfig);
      // String strJsonConfig = String(jsonConfig);
      // Serial.println(strJsonConfig);
      request->send(200, "application/json", jsonConfig);
    }
    catch (std::exception err)
    {
      Serial.println(err.what());
    }
  });

  server.onNotFound(notFound);
  server.begin();

  heaterOperator.PutServoNeutral();
  //Decreasing CPU Frequency to 40 to save battery
  ets_update_cpu_frequency(80);

  // delay(5000);
  // WiFi.disconnect(true);
  // esp_sleep_enable_timer_wakeup(1000*120);
  // delay(2000);
  // esp_light_sleep_start();
  // esp_deep_sleep_start();
}

void loop()
{
  ArduinoOTA.handle();
  heaterOperator.LoopProcessor();
}
