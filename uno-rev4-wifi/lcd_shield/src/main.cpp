#include <Arduino.h>
#include <Console.h>
#include <U8glib.h>
#include <Clock.h>


// U8G2_ST7565_NHD_C12864_1_4W_HW_SPI u8g2(U8G2_R0, PIN_D10, PIN_D9, PIN_D8);
// U8G2_ST7565_NHD_C12864_1_4W_SW_SPI u8g2(U8G2_R0, D13, D11, D10, D8);
U8GLIB_NHD_C12864 u8g2( D13, D11, D10, 9, PIN_D8);
Clock myClock;
Console console;

int timeUpdate;

void initWifi();

float humidity = 0;

void draw(void)
{
    // graphic commands to redraw the complete screen should be placed here
    // 128x64 pixel
    u8g2.setFont(u8g_font_6x13r);
    auto time = myClock.getTime();
    char buffer[100];
    sprintf(buffer, "%02d:%02d:%02d", time.getHour(), time.getMinutes(), time.getSeconds());
    u8g2.drawStr(80, 10, buffer);
    
    console.draw(u8g2);
    // u8g2.println("Yo 2");
    // u8g2.println("Yo 3");
    // u8g2.println("Yo 4");


    // u8g2.drawStr(0, 8, "Hello");
    // u8g2.drawStr(0, 8+13, "World");
}
void setup()
{
    u8g2.begin();
    u8g2.setContrast(0); // Config the contrast to the best effect
    // initWifi();
    // myClock.begin();
    // myClock.setTimeZone(2);
    // myClock.update();
}

int x = 0;
void loop()
{

    u8g2.firstPage();
    do
    {
        // u8g.println("Humidity: " + String(humidity) + " %");

        draw();
    } while (u8g2.nextPage());

    if (timeUpdate == 0 || millis() - timeUpdate > 1000)
    {
        timeUpdate = millis();
    }
    x++;
    if (x > 20) {
        console.clear();
        x = 0;
    }
    console.println(("Cool " + String(x)).c_str());
    delay(1000);
}
