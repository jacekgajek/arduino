#include "MenuWidget.h"

void MenuWidget::updateEntryCount()
{
    int count = 0;
    for (byte i = 0; i < maxEntryCount; i++)
    {
        if (strlen(entries[i]) > 0) {
            count++;
        }
    }
    entryCount = count;
}

void MenuWidget::setEntry(byte index, char *label)
{
    if (index >= 0 && index < maxEntryCount) {
        strncpy(entries[index], label, CONSOLE_WIDTH);
        entries[index][CONSOLE_WIDTH] = '\0';
    }
}

void MenuWidget::setEntries(char** labels, byte count)
{
    entryCount = 0;
    for (byte i = 0; i < count && i < maxEntryCount; i++)
    {
        setEntry(i, labels[i]);
    }
    updateEntryCount();
}

void MenuWidget::handler(byte index, menu_callback func)
{
    if (index >= 0 && index < entryCount) {
        handlers[index] = func;
    }
}

void MenuWidget::draw(U8GLIB &display)
{
    for (size_t i = 0; i < entryCount; i++)
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

void MenuWidget::moveUp()
{
    selectedEntry = (selectedEntry - 1);
    if (selectedEntry < 0)
    {
        selectedEntry = entryCount - 1;
    }
}

void MenuWidget::moveDown()
{
    selectedEntry = (selectedEntry + 1) % entryCount;
}

void MenuWidget::enter()
{
    if (handlers[selectedEntry] != nullptr)
    {
        handlers[selectedEntry]();
    }
}
