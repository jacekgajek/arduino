#include "Clock.h"

void Clock::begin() {
    _timeClient.begin();
}

void Clock::setTimeZone(int timeZoneOffsetHours)
{
    _timeZoneOffsetHours = timeZoneOffsetHours;
}

bool Clock::update()
{
    bool success = _timeClient.forceUpdate();

    if (success)
    {
        auto epochTime = _timeClient.getEpochTime() + (_timeZoneOffsetHours * 3600);
        auto time = RTCTime(epochTime);

        RTC.setTime(time);
    }
    return success;
}

String Clock::getFormattedTime() {
    RTCTime time;
    RTC.getTime(time);
    return time.toString();
}

void Clock::setAlarm(const AlarmTime &alarmTime, rtc_cbk_t callback) {
    AlarmMatch alarmMatch;
    auto alarmRtcTime = RTCTime(1, Month::JANUARY, 2000, alarmTime.hour, alarmTime.minute, 0, DayOfWeek::MONDAY, SaveLight::SAVING_TIME_INACTIVE);

    alarmMatch.addMatchHour();
    alarmMatch.addMatchMinute();
    alarmMatch.addMatchSecond();

    _alarmTime = alarmTime;
    _alarmSet = true;

    RTC.setAlarmCallback(callback, alarmRtcTime, alarmMatch);
}

void Clock::disableAlarm()
{
    AlarmMatch alarmMatch;
    RTCTime tempTime;
    RTC.setAlarmCallback(nullptr, tempTime, alarmMatch);
    _alarmSet = false;
    _alarmTime = {0, 0};
}

String Clock::getFormattedAlarm()
{
    auto hourPrefix = _alarmTime.hour < 10 ? "0" : "";
    auto minutePrefix = _alarmTime.minute < 10 ? "0" : "";
    return hourPrefix + String(_alarmTime.hour) + ":" + minutePrefix + String(_alarmTime.minute);
}

bool Clock::isAlarmSet()
{
    return _alarmSet;
}

RTCTime Clock::getTime()
{
    RTCTime time;
    RTC.getTime(time);
    return time;
}
