#include "Menu.h"

void Menu::setEntry(byte index, char *label)
{
    if (index > 0 && index < maxEntryCount) {
        strcpy(entries[index], label);
    }
}

void Menu::setEntries(char** labels, byte count)
{
    for (byte i = 0; i < count && i < maxEntryCount; i++)
    {
        strcpy(entries[i], labels[i]);
    }
}

void Menu::handler(byte index, menu_callback func)
{
    if (index > 0 && index < maxEntryCount) {
        handlers[index] = func;
    }
}

void Menu::draw(U8GLIB &display)
{
    for (size_t i = 0; i < maxEntryCount; i++)
    {
        if (strlen(entries[i]) > 0)
        {
            if (selectedEntry == i)
            {
                display.drawBox(0, 13 + i * 13, 128, 13);
                display.setColorIndex(0);
            }
            display.drawStr(0, 23 + i * 13, entries[i]);
            display.setColorIndex(1);
        }
    }
}

void Menu::moveUp()
{
    selectedEntry = (selectedEntry - 1);
    if (selectedEntry < 0)
    {
        selectedEntry = maxEntryCount - 1;
    }
}

void Menu::moveDown()
{
    selectedEntry = (selectedEntry + 1) % maxEntryCount;
}

void Menu::enter()
{
    if (handlers[selectedEntry] != nullptr)
    {
        handlers[selectedEntry]();
    }
}
