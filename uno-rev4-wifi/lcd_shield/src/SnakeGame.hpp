#pragma once

#include <Arduino.h>
#include <vector>
#include <algorithm>
#include <LcdShieldJoystick.h>
#include <U8glib.h>

enum GameState
{
    PAUSE_MESSAGE,
    GAME_OVER_MESSAGE,
    RUNNING,
};

enum GameResultType
{
    GAME_OVER,
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
    static const int initialUpdateDelay = 300;

    static const int resultPrintTime = 3000;
    int snakeThickness = 2;

    Joystick &joystick;
    U8GLIB &u8g;

    struct vector2d
    {
        int x;
        int y;
        bool operator==(const vector2d &other) const
        {
            return other.x == x && other.y == y;
        }
    };

    int debounce = 200;
    int lastDebounce = 0;
    int length = 1;

    int screenHeight;
    int screenWidth;
    int score = 1;

    int lastUpdate = 0;
    int updateDelay = initialUpdateDelay;

    int resultPrintStart = 0;

    GameState state;
    GameResult result;

    vector2d moveDirection = {1, 0};
    vector2d previousDirection = moveDirection;
    vector2d foodPosition;

    std::vector<vector2d> snakeBody;

    bool draw();
    void drawSnake();
    void drawFrame();
    void drawFood();
    bool drawResult();

    void createFood();
    bool move();

    void readJoystick();

    GameResultType stateToResultType(GameState state) const;

public:
    SnakeGame(Joystick &joystick, U8GLIB &u8g);
    bool gameLoop();
    void begin();
    void resume();
    GameResult getResult() const { return result; }
};

