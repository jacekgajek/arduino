#pragma once
#include <U8glib.h>

class SignalStrengthWidget
{
    private:
        int signalStrength = 0;
    public:
        void setSignalStrength(int strength) { signalStrength = strength; }
        void draw(U8GLIB &display, int x, int y)
        {
            switch (signalStrength)
            {
            case 4:
                display.drawLine(x + 8, y - 8, x + 8, y);
            case 3:
                display.drawLine(x + 6, y - 6, x + 6, y);
            case 2:
                display.drawLine(x + 4, y - 4, x + 4, y);
            case 1:
                display.drawLine(x + 2, y - 2, x + 2, y);
            case 0:
                display.drawPixel(x, y);
            default:
                break;
            }
        }
};
