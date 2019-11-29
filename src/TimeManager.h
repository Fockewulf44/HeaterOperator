
#include <Arduino.h>

class TimeManager
{
    public:
    TimeManager(int periodOfSync, int16_t gmtHoursOffset);
    ~TimeManager();
    bool IsSynchronized;
    void TimeUpdate();

    private:
    int _periodOfSync;
    bool _isSynchronized;
    int16_t _gmtHoursOffset;
};