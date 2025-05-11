#include <includes.h>
#include <wifi_secrets.h>

char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASSWORD;

void initWifi(WiFiServer &server)
{
    int status = WL_IDLE_STATUS;

    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println("Communication with WI-FI module failed!");
        while (true)
            ;
    }

    String fv = WiFi.firmwareVersion();
    Serial.print("Firmware version: ");
    Serial.println(fv);
    if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    {
        Serial.println("Please upgrade the firmware");
    }

    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
        delay(100);
    }
    Serial.println("You're connected to the network");

    server.begin();
    Serial.println("Server started. Printing connection data.");
    printWifiData();
}

void triggerPrintingWifiData()
{
    printInfo = true;
}

// void triggerBuzzerSwitch()
// {
    // buzzer.setIsActive(!buzzer.isActive());
// }

// void triggerBuzzerStart()
// {
    // buzzer.activate();
// }

void initPins()
{
    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);

    attachInterrupt(BUTTON_UP_PIN, triggerPrintingWifiData, RISING);

}

void initSerial() {

    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    Serial.println("Hello setup!");
}

void initTime() {
    myClock.setTimeZone(TIME_ZONE_OFFSET_HOURS);
    Serial.println("Fetching current time...");
    myClock.begin();
    myClock.update();
    Serial.print("Current time: ");
    Serial.println(myClock.getFormattedTime());
}

void initAlarm() {
    // buzzer.setDelay(300);
    // buzzer.setDuration(50);

    AlarmTime alarmTime;
    EEPROM.get(0, alarmTime);
    if (alarmTime.hour >= 0 && alarmTime.hour < 24 &&
        alarmTime.minute >= 0 && alarmTime.minute < 60) {
        // myClock.setAlarm(alarmTime, triggerBuzzerStart);
        Serial.println("Alarm set on: " + myClock.getFormattedAlarm() + " (UTC+" + String(TIME_ZONE_OFFSET_HOURS) + ")");
    }
}

void init()
{
    initPins();
    initSerial();
    initWifi(server);

    dht.begin();
    RTC.begin();

    initTime();
    initAlarm();
}
