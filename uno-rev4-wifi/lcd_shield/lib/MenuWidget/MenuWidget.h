#pragma once

#include "Arduino.h"
#include "U8glib.h"

#ifndef CONSOLE_WIDTH
#define CONSOLE_WIDTH 20
#endif
#ifndef CONSOLE_HEIGHT
#define CONSOLE_HEIGHT 5
#endif

static const int maxEntryCount = CONSOLE_HEIGHT - 1;

typedef void (*menu_callback)(void); // type for conciseness

class MenuWidget {

private:
    char entries[maxEntryCount][CONSOLE_WIDTH + 1] = { {""},
                                                       {""},
                                                       {""},
                                                       {""}};

    menu_callback handlers[maxEntryCount] = {nullptr};
    int selectedEntry = 0;
    int entryCount = 0;

    void updateEntryCount();
    void setEntry(byte index, char *label);

public:
    void setEntries(char** labels, byte count);
    void handler(byte index, menu_callback func);
    void draw(U8GLIB &display);
    void moveUp();
    void moveDown();
    void enter();
};
