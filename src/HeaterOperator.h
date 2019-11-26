
#include <Arduino.h>
#include "Servo.h"

class HeaterOperator
{
    public:
    HeaterOperator(uint16_t servoPin, uint16_t dthPin);
    ~HeaterOperator();
    void ManageHeater();
    void ProcessCommand(uint8_t*);
    void SyncTime();
    int GetDateTime();
    int Schedule1TurnOn;
    int Schedule1TurnOff;
    int Schedule2TurnOn;
    int Schedule2TurnOff; 

    private:
    int currentDateTime;
    int dateTimeUpdDtm;
    uint16_t servoPin;
    uint16_t dthPin;
    uint8_t isHeaterEnabled;
    uint8_t isHeaterHigh;
    uint8_t isStartedBySchedule;
    Servo servo1;
};