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

class Menu {

private:
    char entries[maxEntryCount][CONSOLE_WIDTH + 1] = {{" Play Snake"},
                                                      {" Reset clock"},
                                                      {" Reconnect"},
                                                      {" Reset"}};


    menu_callback handlers[maxEntryCount] = {nullptr, nullptr, nullptr, nullptr};
    int selectedEntry = 0;

public:
    void setEntry(byte index, char *label);
    void setEntries(char** labels, byte count);
    void handler(byte index, menu_callback func);
    void draw(U8GLIB &display);
    void moveUp();
    void moveDown();
    void enter();
};
