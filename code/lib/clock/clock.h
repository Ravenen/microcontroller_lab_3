#include <stdio.h>

struct Time {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
};
enum FunctionModes {
    NONE,
    SET_TIME,
    SET_ALARM,
    SHOW_ALARM
};

class Clock {
private:
    char time_buffer[8];
    uint8_t time_buffer_offset;
    bool validateTime(Time _time);
public:
    Time time = { 0, 0, 0 };
    Time alarm = { 0, 0, 0 };
    bool enable_alarm = false;
    const uint8_t alarm_duration_in_sec = 30;
    FunctionModes function_mode = NONE;
    bool setTime(Time _time);
    bool setTime(char* _time_str);
    bool setAlarm(Time _alarm);
    bool setAlarm(char* _alarm_str);
    bool setByFunctionMode(Time value);
    bool setByFunctionMode(char* value_str);
    void toggleAlarm();
};