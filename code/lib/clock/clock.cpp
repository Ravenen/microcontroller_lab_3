#include "clock.h"

Time stringToTime(char* time_string);

bool Clock::validateTime(Time _time) {
    return _time.hours >= 0 && _time.hours <= 23 &&
        _time.minutes >= 0 && _time.minutes <= 59 &&
        _time.seconds >= 0 && _time.seconds <= 59;
}

bool Clock::setTime(Time _time) {
    if (validateTime(_time)) {
        time = _time;
        return true;
    }
    else {
        return false;
    }
}

bool Clock::setTime(char* _time_str) {
    return setTime(stringToTime(_time_str));
}

bool Clock::setAlarm(Time _alarm) {
    if (validateTime(_alarm)) {
        alarm = _alarm;
        return true;
    }
    else {
        return false;
    }
}

bool Clock::setAlarm(char* _alarm_str) {
    return setAlarm(stringToTime(_alarm_str));
}

bool Clock::setByFunctionMode(Time value) {
    if (function_mode == SET_TIME) {
        return setTime(value);
    }
    if (function_mode == SET_ALARM) {
        return setAlarm(value);
    }
    return false;
}

bool Clock::setByFunctionMode(char* value_str) {
    if (function_mode == SET_TIME) {
        return setTime(value_str);
    }
    if (function_mode == SET_ALARM) {
        return setAlarm(value_str);
    }
    return false;
}

void Clock::toggleAlarm() {
    enable_alarm = !enable_alarm;
}

// format: hhmm, where h - hours, m - minutes
Time stringToTime(char* time_string) {
    Time result = { 0, 0, 0 };
    result.hours = (time_string[0] - '0') * 10 + (time_string[1] - '0');
    result.minutes = (time_string[2] - '0') * 10 + (time_string[3] - '0');
    return result;
}