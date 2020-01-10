
#include "Arduino.h"
#include <ESP8266WiFi.h>

class WiFiScheduler
{
private:
    /* data */
public:
    struct tm WiFiTurnOn;
    struct tm WiFiTurnOff;
    int WifiTurnOnCycleMin;
    int WifiTurnOffCycleMin;
    bool IsWiFiCycleTurnOn;
    WiFiScheduler(/* args */);
    void LoopProcessor();
    ~WiFiScheduler();
};

WiFiScheduler::WiFiScheduler(/* args */)
{
    //predefining WiFi turnOn and Off
    WiFiTurnOff.tm_hour = 1;
    WiFiTurnOff.tm_min = 0;
    WiFiTurnOn.tm_hour = 8;
    WiFiTurnOn.tm_min = 0;
    IsWiFiCycleTurnOn = false;
    WifiTurnOnCycleMin = 0;
    WifiTurnOffCycleMin = 5;
}

void WiFiScheduler::LoopProcessor()
{
    time_t now;
    struct tm *timeinfo;
    time(&now);
    timeinfo = localtime(&now);
    if (IsWiFiCycleTurnOn)
    {
        if (timeinfo->tm_min == WifiTurnOnCycleMin)
        {
            
        }

        if (timeinfo->tm_min == WifiTurnOffCycleMin)
        {
            WiFi.disconnect();
        }
    }
}

WiFiScheduler::~WiFiScheduler()
{
}
