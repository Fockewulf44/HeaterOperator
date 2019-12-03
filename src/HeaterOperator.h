
#include <Arduino.h>
#include "Servo.h"

class HeaterOperator
{
    public:
    HeaterOperator(uint16_t servoPin, uint16_t dthPin);
    ~HeaterOperator();
    void LoopProcessor();
    void ProcessCommand(uint8_t*);
    void SyncTime();
    int GetDateTime();
    struct tm Schedule1TurnOn;
    struct tm Schedule1TurnOff;
    bool IsSchedule1Enabled;
    bool IsSchedule1Activated;
    struct tm Schedule2TurnOn;
    struct tm Schedule2TurnOff;
    bool IsSchedule2Enabled;
    bool IsSchedule2Activated;
    void TurnOnHeater();
    void TurnOffHeater();
    void UpdateHeaterPower();
    void PutServoNeutral();
    void SetConfig(uint8_t*);
    void BlinkBlueLed();
    char* GetConfig();

    private:
    int currentDateTime;
    int dateTimeUpdDtm;
    uint16_t servoPin;
    uint16_t dthPin;
    bool isHeaterEnabled;
    bool isHeaterHigh;
    bool isStartedBySchedule;
    bool isTestOn;
    Servo servo1;
};