
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
    int Schedule1TurnOn;
    int Schedule1TurnOff;
    int Schedule2TurnOn;
    int Schedule2TurnOff; 
    void TurnOnHeater();
    void UpdateHeaterPower(bool IsHigh);
    void PutServoNeutral();

    private:
    int currentDateTime;
    int dateTimeUpdDtm;
    uint16_t servoPin;
    uint16_t dthPin;
    bool isHeaterEnabled;
    bool isHeaterHigh;
    bool isStartedBySchedule;
    
    Servo servo1;
};