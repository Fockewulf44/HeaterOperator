
#include "HeaterOperator.h"
#include "Servo.h"
#include <Arduino.h>
#include "ArduinoJson.h"

// Servo servo1;

HeaterOperator::HeaterOperator(uint16_t servoPin, uint16_t dthPin)
{
  this->servoPin = servoPin;
  this->dthPin = dthPin;
  this->servo1 = Servo();

  this->isHeaterEnabled = false;
  this->isHeaterHigh = false;
  this->isStartedBySchedule = false;
  this->IsSchedule1Enabled = false;
  this->IsSchedule1Activated = false;
  this->IsSchedule2Enabled = false;
  this->IsSchedule2Activated = false;

  //Predefining manually
  struct tm predefined;
  predefined.tm_hour = 06;
  predefined.tm_min = 40;
  this->Schedule1TurnOn = predefined;
  this->IsSchedule1Enabled = true;
}

HeaterOperator::~HeaterOperator()
{
  this->servo1.~Servo();
}

void HeaterOperator::TurnOnHeater()
{
  servo1.attach(servoPin);
  servo1.write(50);
  delay(200);
  servo1.write(90);
  delay(400);
  this->servo1.detach();
  isHeaterEnabled = true;
  isHeaterHigh = false;
}

void HeaterOperator::TurnOffHeater()
{
  servo1.attach(servoPin);
  servo1.write(50);
  delay(200);
  servo1.write(90);
  delay(400);
  this->servo1.detach();
  isHeaterEnabled = false;
  isHeaterHigh = false;
}

void HeaterOperator::UpdateHeaterPower()
{
  if (isHeaterEnabled)
  {
    servo1.attach(servoPin);
    this->servo1.write(140);
    delay(200);
    this->servo1.write(90);
    delay(400);
    this->servo1.detach();
    isHeaterHigh = !isHeaterHigh;
  }
}

void HeaterOperator::PutServoNeutral()
{
  servo1.attach(servoPin);
  servo1.write(100);
  delay(400);
  servo1.detach();
  delay(50);
  servo1.attach(servoPin);
  servo1.write(90);
  delay(400);
  servo1.detach();
}

void HeaterOperator::ProcessCommand(uint8_t *data)
{
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject((const char *)data);
  if (root.success())
  {
    if (root.containsKey("turnOnHeater"))
    {
      TurnOnHeater();
    }
    if (root.containsKey("turnOffHeater"))
    {
      TurnOffHeater();
    }
    if (root.containsKey("write"))
    {
      //Just for test
      servo1.attach(servoPin);

      servo1.write(root["write"]);
      delay(200);
      servo1.write(90);
      delay(400);
      this->servo1.detach();
    }
    if (root.containsKey("detach"))
    {
      Serial.println(root["detach"].asString());
      servo1.detach();
    }
    if (root.containsKey("attach"))
    {
      Serial.println(root["attach"].asString());
      servo1.attach(servoPin);
    }
  }
  else
  {
  }
}

void HeaterOperator::SetConfig(uint8_t *data)
{
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject((const char *)data);
  if (root.success())
  {
    if (root.containsKey("schedules"))
    {
      Serial.println("schedules");
      JsonObject &schedules = root["schedules"];
      if (schedules.containsKey("schedule1"))
      {
        Serial.println("schedule1");
        JsonObject &schedule1 = schedules["schedule1"];
        IsSchedule1Enabled = schedule1["isEnabled"] == 1 ? true : false;
        JsonObject &turnOn = schedule1["turnOn"];
        Schedule1TurnOn.tm_hour = turnOn["hour"];
        Schedule1TurnOn.tm_min = turnOn["min"];
        Schedule1TurnOn.tm_sec = 0;

        JsonObject &turnOff = schedule1["turnOff"];
        Schedule1TurnOff.tm_hour = turnOff["hour"];
        Schedule1TurnOff.tm_min = turnOff["min"];
        Schedule1TurnOff.tm_sec = 0;
      }
      if (schedules.containsKey("schedule2"))
      {
      }
    }
  }
}

void HeaterOperator::GetConfig(char* outputJSON)
{
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject &root = JSONbuffer.createObject();
  JsonObject &Schedules = JSONbuffer.createObject();
  JsonObject &Schedule1 = JSONbuffer.createObject();
  JsonObject &turnOn = JSONbuffer.createObject();
  JsonObject &turnOff = JSONbuffer.createObject();
  turnOn["hour"] = Schedule1TurnOn.tm_hour;
  turnOn["min"] = Schedule1TurnOn.tm_min;
  turnOn["sec"] = Schedule1TurnOn.tm_sec;
  turnOff["hour"] = Schedule1TurnOff.tm_hour;
  turnOff["min"] = Schedule1TurnOff.tm_min;
  turnOff["sec"] = Schedule1TurnOff.tm_sec;
  Schedule1["turnOn"] = turnOn;
  Schedule1["turnOff"] = turnOff;
  Schedules["schedule1"] = Schedule1;
  root["schedules"] = Schedules;

  root.printTo(outputJSON, 300);
  // Serial.println(outputJSON);
}

void HeaterOperator::BlinkBlueLed()
{
  // delay(100);
  // digitalWrite(LED, HIGH);
  // delay(100);
  // digitalWrite(LED, LOW);
}

void HeaterOperator::LoopProcessor()
{
  try
  {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
      if (IsSchedule1Enabled)
      {
        if (timeinfo.tm_hour == Schedule1TurnOn.tm_hour && timeinfo.tm_min == Schedule1TurnOn.tm_min && timeinfo.tm_sec == 00)
        {
          if (IsSchedule1Activated == false)
          {
            this->IsSchedule1Activated = true;
            TurnOnHeater();
            UpdateHeaterPower();
            Serial.println("Turning on Heater");
          }
        }
        else
        {
          IsSchedule1Activated = false;
        }
      }
    }
  }
  catch (std::exception err)
  {
    Serial.println(err.what());
  }
}