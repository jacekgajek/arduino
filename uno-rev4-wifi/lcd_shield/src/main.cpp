#include <Arduino.h>
#include <Console.h>
#include <U8glib.h>
#include <Clock.h>
#include <RTC.h>
#include <WiFi.h>
#include <SignalStrengthWidget.h>

// U8G2_ST7565_NHD_C12864_1_4W_HW_SPI u8g2(U8G2_R0, PIN_D10, PIN_D9, PIN_D8);
// U8G2_ST7565_NHD_C12864_1_4W_SW_SPI u8g2(U8G2_R0, D13, D11, D10, D8);
U8GLIB_NHD_C12864 u8g(D13, D11, D10, 9, PIN_D8);
Clock myClock;
Console console;
SignalStrengthWidget signalStrengthWidget;

int timeUpdate;

void initWifi(Console &console);
int signalStrength();

float humidity = 0;

void draw(void)
{
    // 128x64 pixel
    u8g.setFont(u8g_font_6x13r);
    auto time = myClock.getTime();
    char buffer[100];
    sprintf(buffer, "%02d:%02d:%02d", time.getHour(), time.getMinutes(), time.getSeconds());
    u8g.drawStr(0, 10, buffer);
    console.draw(u8g);

    signalStrengthWidget.setSignalStrength(signalStrength());
    signalStrengthWidget.draw(u8g, 118, 8);
}

void setup()
{
    RTC.begin();
    u8g.begin();
    u8g.setContrast(0);
    initWifi(console);
    myClock.begin();
    myClock.setTimeZone(2);
}

int x = 0;
int consoleCleared = 5000;
void loop()
{

    u8g.firstPage();
    do
    {
        draw();
    } while (u8g.nextPage());

    if (timeUpdate == 0 || millis() - timeUpdate > 10000)
    {
        timeUpdate = millis();
        myClock.update();
    }
    x++;
    if (x > 20) {
        x = 0;
    }
    delay(100);
    if (consoleCleared > 0 && millis() > consoleCleared) {
        console.clear();
        consoleCleared = 0;
    }
}
