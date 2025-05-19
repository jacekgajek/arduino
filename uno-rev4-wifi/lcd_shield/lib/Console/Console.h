#pragma once
#include <U8glib.h>
#include <Arduino.h>

#ifndef CONSOLE_WIDTH
#define CONSOLE_WIDTH 20
#endif
#ifndef CONSOLE_HEIGHT
#define CONSOLE_HEIGHT 5
#endif

class Console
{
private:
    static const int TOP_OFFSET = 20;
    static const int WIDTH = CONSOLE_WIDTH + 1;
    static const int HEIGHT = CONSOLE_HEIGHT;

    char consoleBuffer[HEIGHT][WIDTH];
    int consoleLine = 0;

public:
    Console()
    {
        clear();
    }
    void println(const String& str) {
        println(str.c_str());
    }

    void println(const char *message)
    {
        if (consoleLine < HEIGHT)
        {
            strncpy(consoleBuffer[consoleLine], message, WIDTH - 1);
            consoleBuffer[consoleLine][WIDTH - 1] = '\0';
            consoleLine++;
        }
        else
        {
            memmove(consoleBuffer[0], consoleBuffer[1], (HEIGHT - 1) * WIDTH);
            strncpy(consoleBuffer[HEIGHT - 1], message, WIDTH - 1);
            consoleBuffer[HEIGHT - 1][WIDTH - 1] = '\0';
        }
    }

    void clear(void)
    {
        memset(consoleBuffer, 0, sizeof(consoleBuffer));
        consoleLine = 0;
    }

    void draw(U8GLIB &display)
    {
        display.setFont(u8g_font_6x13r);
        for (int i = 0; i < HEIGHT; i++)
        {
            display.drawStr(0, TOP_OFFSET + (i * 10), consoleBuffer[i]);
        }
    }
};
