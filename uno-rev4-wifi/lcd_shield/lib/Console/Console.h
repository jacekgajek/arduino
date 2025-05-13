#pragma once
#include <U8glib.h>

class Console
{
private:
    static const int WIDTH = 20;
    static const int HEIGHT = 6;

    char consoleBuffer[HEIGHT][WIDTH];
    int consoleLine = 0;

public:
    Console()
    {
        clear();
    }

    void println(const char *message)
    {
        if (consoleLine < HEIGHT)
        {
            strcpy(consoleBuffer[consoleLine], message);
            consoleLine++;
        }
        else
        {
            for (int i = 0; i < HEIGHT - 1; i++)
            {
                strcpy(consoleBuffer[i], consoleBuffer[i + 1]);
            }
            strcpy(consoleBuffer[HEIGHT - 1], message);
        }
    }

    void clear(void)
    {
        for (int i = 0; i < HEIGHT; i++)
        {
            strcpy(consoleBuffer[i], "");
        }
        consoleLine = 0;
    }

    void draw(U8GLIB_NHD_C12864 &display)
    {
        display.setFont(u8g_font_6x13r);
        for (int i = 0; i < HEIGHT; i++)
        {
            display.drawStr(0, 10 + (i * 10), consoleBuffer[i]);
        }
    }
};