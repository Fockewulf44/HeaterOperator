

#include "ArduinoJson.h"
#include "TimeManager.h"

TimeManager::TimeManager(int periodOfSync, int16_t gmtHoursOffset)
{
    this->_periodOfSync = periodOfSync;
    this->_gmtHoursOffset = gmtHoursOffset;

    this->_isSynchronized = false;
}

void TimeManager::TimeUpdate()
{
    if (!_isSynchronized)
    {

    }
    else
    {

    }
}