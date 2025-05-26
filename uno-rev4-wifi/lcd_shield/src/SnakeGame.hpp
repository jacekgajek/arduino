#pragma once

#include <Arduino.h>
#include <vector>
#include <LcdShieldJoystick.h>
#include <U8glib.h>

enum GameState
{
    WIN_MESSAGE,
    LOSE_MESSAGE,
    EXIT_MESSAGE,
    RUNNING,
};

enum GameResultType
{
    WIN,
    LOSE,
    PAUSED,
};

struct GameResult
{
    GameResultType result;
    int score;
};

class SnakeGame
{
private:
    static const int initialUpdateDelay = 100;
    static const int resultPrintTime = 5000;

    LcdShieldJoystick &joystick;
    U8GLIB &u8g;

    struct vector2d
    {
        int x;
        int y;
    };

    int debounce = 200;
    int lastDebounce = 0;
    int length = 1;

    int screenHeight;
    int screenWidth;

    int lastUpdate = 0;
    int updateDelay = initialUpdateDelay;

    int resultPrintStart = 0;

    GameState state;
    GameResult result;

    vector2d moveDirection;

    std::vector<vector2d> snakeBody;

    void drawSnake();
    void drawFrame();
    bool move();
    bool drawResult();
    GameResultType stateToResultType(GameState state) const;

public:
    SnakeGame(LcdShieldJoystick &joystick, U8GLIB &u8g);
    bool gameLoop();
    void reset();
    void resume();
    GameResult getResult() const { return result; }
};

