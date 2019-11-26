
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

  Serial.begin(115200);
  PutServoNeutral();
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
      Serial.println(root["cmd1"].asString());

      servo1.write(120);
      delay(1000);
      servo1.write(root["turnOnHeater"]);
    }
    if (root.containsKey("write"))
    {
      Serial.println(root["write"].asString());
      servo1.write(root["write"]);
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
    //   request->send(200, "text/plain", "");
  }
  else
  {
    //   request->send(404, "text/plain", "");
  }
}

void HeaterOperator::TurnOnHeater()
{
  if (!servo1.attached())
  {
    servo1.attach(servoPin);
  }
  servo1.write(35);
  delay(200);
  servo1.write(90);
  isHeaterEnabled = true;
  isHeaterHigh = false;
  this->servo1.detach();
}

void HeaterOperator::UpdateHeaterPower(bool IsHigh)
{
  if (isHeaterEnabled)
  {
    if (!servo1.attached())
    {
      servo1.attach(servoPin);
    }
    this->servo1.write(125);
    delay(100);
    this->servo1.write(90);
    this->servo1.detach();
  }
}

void HeaterOperator::PutServoNeutral()
{
  if (!servo1.attached())
  {
    servo1.attach(servoPin);
  }
  this->servo1.write(90);
  this->servo1.detach();
}

void HeaterOperator::LoopProcessor()
{
}