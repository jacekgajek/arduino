#pragma once
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <RTC.h>

struct AlarmTime {
    int hour;
    int minute;
};

class Clock {
    private:
    int _timeZoneOffsetHours = 0;
    WiFiUDP _udp;
    NTPClient _timeClient = NTPClient(_udp);
    AlarmTime _alarmTime = {0, 0};
    bool _alarmSet = false;

    public:

    void setTimeZone(int timeZoneOffset);
    void begin();
    bool update();

    void setAlarm(const AlarmTime &alarmTime, rtc_cbk_t callback);
    void disableAlarm();

    String getFormattedAlarm();
    bool isAlarmSet();
    RTCTime getTime();
    String getFormattedTime();
};
