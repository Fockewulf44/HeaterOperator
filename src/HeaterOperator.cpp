
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
}

HeaterOperator::~HeaterOperator()
{
  this->servo1.~Servo();
}

void HeaterOperator::ProcessCommand(uint8_t *data)
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject((const char *)data);
  if (root.success())
  {
    if (root.containsKey("cmd"))
    {
      Serial.println(root["cmd"].asString());
    }
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
      delay(200);
      this->servo1.detach();
    }
    if (root.containsKey("writemilliseconds"))
    {
      Serial.println(root["writemilliseconds"].asString());
      servo1.writeMicroseconds(root["writemilliseconds"]);
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

void HeaterOperator::TurnOnHeater()
{
  servo1.attach(servoPin);
  servo1.write(35);
  delay(200);
  servo1.write(90);
  delay(200);
  this->servo1.detach();
  isHeaterEnabled = true;
  isHeaterHigh = false;
}

void HeaterOperator::TurnOffHeater()
{
  servo1.attach(servoPin);
  servo1.write(35);
  delay(200);
  servo1.write(90);
  delay(200);
  this->servo1.detach();
  isHeaterEnabled = false;
  isHeaterHigh = false;
}

void HeaterOperator::UpdateHeaterPower()
{
  if (isHeaterEnabled)
  {
    if (!servo1.attached())
    {
      servo1.attach(servoPin);
    }
    this->servo1.write(125);
    delay(200);
    this->servo1.write(90);
    delay(200);
    this->servo1.detach();
    isHeaterHigh = !isHeaterHigh;
  }
}

void HeaterOperator::PutServoNeutral()
{
  servo1.attach(servoPin);
  this->servo1.write(90);
  delay(200);
  this->servo1.detach();
}

void HeaterOperator::LoopProcessor()
{
  try
  {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
      if (timeinfo.tm_sec == 55 && isTestOn == false)
      {
        isTestOn = true;
        // TurnOnHeater();
        Serial.println("Turning on Heater");
      }
      else
      {
        isTestOn = false;
      }
    }
  }
  catch (std::exception err)
  {
    Serial.println(err.what());
  }
}