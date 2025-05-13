#include <Arduino.h>
#include <U8g2lib.h>

// U8G2_ST7565_NHD_C12864_1_4W_HW_SPI u8g2(U8G2_R0, PIN_D10, PIN_D9, PIN_D8);
U8G2_ST7565_NHD_C12864_1_4W_SW_SPI u8g2(U8G2_R0, D13, D11, D10, D8);

float humidity = 0;

void draw(void)
{
    // graphic commands to redraw the complete screen should be placed here
    u8g2.setFont(u8g_font_unifont);
    // u8g.setFont(u8g_font_osb21);
    u8g2.drawCircle(64, 32, 20);
    u8g2.drawStr(0, 10, "Hello World!");
}
/*
 uint8_t cs -> SPI Interface: CS
 uint8_t dc -> SPI Interface: CD (guess)
 uint8_t reset -> SPI Interface: RST


 uint8_t clock -> SPI Interface: SCK
 uint8_t data -> SPI Interface: MOSI
 uint8_t cs -> SPI Interface: CD
 uint8_t dc -> SPI Interface: CD (guess)
 uint8_t reset -> SPI Interface: RST
*/
void setup()
{
    u8g2.begin();
    u8g2.setContrast(0); // Config the contrast to the best effect
    // set SPI backup if required
    // u8g.setHardwareBackup(u8g_backup_avr_spi);

    // assign default color value
    // if (u8g.getCols() == U8G_MODE_R3G3B2)
    // {
    //     u8g.setColorIndex(255); // white
    // }
    // else if (u8g.getMode() == U8G_MODE_GRAY2BIT)
    // {
    //     u8g.setColorIndex(3); // max intensity
    // }
    // else if (u8g.getMode() == U8G_MODE_BW)
    // {
    //     u8g.setColorIndex(1); // pixel on
    // }
    // else if (u8g.getMode() == U8G_MODE_HICOLOR)
    // {
    //     u8g.setHiColorByRGB(255, 255, 255);
    // }

    // Serial1.begin(9600);
    // Serial.begin(9600);
    // while (Serial)
    // ;
    // while (Serial1)
    // ;

    // u8g.println("Hello World!");
}

void loop()
{

    // u8g2.clearBuffer();
    // u8g2.setFont(u8g2_font_ncenB14_tr);
    // u8g2.drawStr(0, 20, "Hello World!");
    // u8g2.sendBuffer();

    // // picture loop
    // // Serial.println("Hello!");
    u8g2.firstPage();
    do
    {
        // u8g.println("Humidity: " + String(humidity) + " %");

        draw();
    } while (u8g2.nextPage());

    // // Serial.println("World!");
    // // rebuild the picture after some delay
    // delay(500);
}
