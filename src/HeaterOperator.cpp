
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
  Serial.begin(115200);
  this->servo1.attach(servoPin);
}

HeaterOperator::~HeaterOperator()
{
}

void HeaterOperator::ProcessCommand(uint8_t *data)
{
  servo1.write(180);
  delay(1000);
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
    //   request->send(200, "text/plain", "");
  }
  else
  {
    //   request->send(404, "text/plain", "");
  }
}

void ManageHeater()
{
  
}