#include <wifi_secrets.h>
#include <Arduino.h>
#include <WiFi.h>

char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASSWORD;

void printMacAddress(byte mac[]);
void printCurrentNet();
void printWifiData();

void initWifi()
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

    printWifiData();
}
