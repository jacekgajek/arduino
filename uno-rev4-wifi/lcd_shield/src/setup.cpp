#include <wifi_secrets.h>
#include <Arduino.h>
#include <Console.h>
#include <WiFi.h>

char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASSWORD;



void printMacAddress(byte mac[]);
void printCurrentNet();
void printWifiData();

int signalStrength() {
    int rssi = WiFi.RSSI();

    const int minRSSI = -90; // Weak signal
    const int maxRSSI = -30; // Strong signal
    const int minGrade = 0;
    const int maxGrade = 4;

    // Map RSSI to the scale
    int grade = round(1.0f * (rssi - minRSSI) * (maxGrade - minGrade) / (maxRSSI - minRSSI) + minGrade);

    // Clamp the grade between 0 and 5
    if (grade < minGrade) grade = minGrade;
    if (grade > maxGrade) grade = maxGrade;

    return grade;
}

bool initWifi(Console &console)
{
    int status = WL_IDLE_STATUS;

    if (WiFi.status() == WL_NO_MODULE)
    {
        console.println("WI-FI failed!");
        return false;
    }

    String fv = WiFi.firmwareVersion();
    console.println("ver " + String(fv));
    if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    {
        console.println("Upgrade firmware!");
    }

    while (status != WL_CONNECTED)
    {
        console.println("ssid " + String(ssid));
        status = WiFi.begin(ssid, pass);
        delay(100);
    }
    IPAddress ip = WiFi.localIP();
    console.println("ip " + String(ip.toString()));
    console.println("sig " + String(WiFi.RSSI()));
    console.println("grade " + String(signalStrength()));
}
