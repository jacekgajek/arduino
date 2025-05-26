#define CONSOLE_WIDTH 20
#define CONSOLE_HEIGHT 5

#include <Arduino.h>
#include <LcdShieldJoystick.h>
#include <Console.h>
#include <U8glib.h>
#include <Clock.h>
#include <RTC.h>
#include <WiFi.h>
#include <MenuWidget.h>
#include <SignalStrengthWidget.h>
#include <SnakeGame.hpp>

U8GLIB_NHD_C12864 u8g(D13, D11, D10, 9, PIN_D8);
Clock myClock;
Console console;
SignalStrengthWidget signalStrengthWidget;
MenuWidget menu;
LcdShieldJoystick joystick(A0);
SnakeGame snakeGame(joystick, u8g);

int timeUpdate;

void initWifi(Console &console);
int signalStrength();

float humidity = 0;
int consoleClear = 4000;
int consoleLast = 0;

bool consoleVisible = true;
bool consoleCleared = false;
bool headerVisible = true;
bool snake = false;
bool shouldRestartSnake = true;



void reset() {
    NVIC_SystemReset();
}

char buffer[100];
int debounce = 200;
int lastDebounce = 0;
int lastRedraw = 0;
int redraw = 40;

void draw(void)
{
    if (headerVisible)
    {
        u8g.drawBox(0, 0, 128, 11);
        u8g.setColorIndex(0);
        u8g.setFont(u8g_font_6x13r);
        auto time = myClock.getTime();
        sprintf(buffer, "%02d:%02d:%02d", time.getHour(), time.getMinutes(), time.getSeconds());
        u8g.drawStr(0, 10, buffer);

        signalStrengthWidget.setSignalStrength(signalStrength());
        signalStrengthWidget.draw(u8g, 118, 8);

        u8g.setColorIndex(1);
    }
    if (consoleVisible)
    {
        console.draw(u8g);
    }
    else
    {
        menu.draw(u8g);
    }
}

void handleUpdateClock() {
    myClock.update();
}
void handleReconnect() {
    consoleLast = millis();
    consoleVisible = true;
    initWifi(console);
}
void handleSnake() {
    consoleVisible = false;
    snake = true;
    headerVisible = false;
    if (shouldRestartSnake) {
        snakeGame.reset();
    } else {
        snakeGame.resume();
    }
}

void setup()
{
    RTC.begin();
    u8g.begin();
    u8g.setContrast(0);
    initWifi(console);
    myClock.begin();
    myClock.setTimeZone(2);

    char* entries[4] = { " Play Snake", " Update clock", " Reconnect", " Reset"};
    menu.setEntries(entries, 4);
    menu.handler(0, handleSnake);
    menu.handler(1, handleUpdateClock);
    menu.handler(2, handleReconnect);
    menu.handler(3, reset);

    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
}


void menuLoop();

void loop()
{
    if (snake)
    {
        bool running = snakeGame.gameLoop();
        if (!running)
        {
            snake = false;
            headerVisible = true;
            consoleVisible = true;
            consoleLast = millis();
            if (snakeGame.getResult().result == PAUSED) {
                console.println("Game paused!");
                console.println("Current score: " + String(snakeGame.getResult().score));
                shouldRestartSnake = false;
            } else {
                console.println("Snake game finished!");
                console.println("Final score: " + String(snakeGame.getResult().score));
                shouldRestartSnake = true;
            }
        }
    }
    else
    {
        menuLoop();
    }
}


void menuLoop()
{
    int now = millis();
    if (now - lastRedraw > redraw)
    {
        lastRedraw = now;
        u8g.firstPage();
        do
        {
            draw();
        } while (u8g.nextPage());
    }

    if (timeUpdate == 0 || now - timeUpdate > 3600000)
    {
        timeUpdate = now;
        myClock.update();
    }
    if (!consoleVisible && !consoleCleared)
    {
        console.clear();
        consoleCleared = true;
    }
    else if (consoleVisible && now - consoleLast > consoleClear)
    {
        consoleVisible = false;
        consoleCleared = false;
    }
    auto joy = joystick.read();

    if (joy != NONE && now - lastDebounce > debounce)
    {
        lastDebounce = now;
        switch (joy)
        {
        case UP:
            menu.moveUp();
            break;
        case DOWN:
            menu.moveDown();
            break;
        case LEFT:
            console.println("LEFT");
            break;
        case RIGHT:
            console.println("RIGHT");
            break;
        case CENTER:
            menu.enter();
            break;
        default:
            break;
        }
    }
}
