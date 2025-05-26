#include <SnakeGame.hpp>

SnakeGame::SnakeGame(LcdShieldJoystick &joystick, U8GLIB &u8g) : joystick(joystick), u8g(u8g) {
    screenHeight = u8g.getHeight();
    screenWidth = u8g.getWidth();

    state = GameState::RUNNING;
    moveDirection = {1, 0};
}

void SnakeGame::resume() {
    state = GameState::RUNNING;
    result = {GameResultType::PAUSED, 0};
    lastDebounce = millis();
    lastUpdate = millis();
    resultPrintStart = 0;
}

void SnakeGame::reset()
{
    moveDirection = {1, 0};
    snakeBody.clear();
    vector2d headPosition { screenWidth / 2, screenHeight / 2 };
    snakeBody.push_back(headPosition);
    length = 1;
    updateDelay = initialUpdateDelay;

    resume();
}

bool SnakeGame::gameLoop()
{
    auto joy = joystick.read();
    bool running = true;
    if (joy != NONE && millis() - lastDebounce > debounce)
    {
        lastDebounce = millis();
        switch (joy)
        {
        case UP:
            moveDirection = {0, -1};
            break;
        case DOWN:
            moveDirection = {0, 1};
            break;
        case LEFT:
            moveDirection = {-1, 0};
            break;
        case RIGHT:
            moveDirection = {1, 0};
            break;
        case CENTER:
            state = EXIT_MESSAGE;
            break;
        default:
            break;
        }
    }

    u8g.firstPage();
    do
    {
        if (state != GameState::RUNNING)
        {
            if (resultPrintStart == 0)
            {
                resultPrintStart = millis();
            }
            if (!drawResult())
            {
                result.score = length;
                result.result = stateToResultType(state);
                running = false;
            }
        }
        else
        {
            drawFrame();
            if (millis() - lastUpdate > updateDelay)
            {
                lastUpdate = millis();
                if (!move())
                {
                    state = LOSE_MESSAGE;
                }
            }
            drawSnake();
        }
    } while (u8g.nextPage());

    return running;
}

void SnakeGame::drawSnake()
{
    for (auto &&snakePart : snakeBody)
    {
        u8g.drawBox(snakePart.x, snakePart.y, 2, 2);
    }
}

void SnakeGame::drawFrame()
{
    u8g.drawHLine(0, 0, screenWidth);
    u8g.drawHLine(0, screenHeight - 1, screenWidth);
    u8g.drawVLine(0, 0, screenHeight);
    u8g.drawVLine(screenWidth - 1, 0, screenHeight);
}

bool SnakeGame::move()
{
    vector2d newHead(snakeBody.front());
    newHead.x += moveDirection.x * 2;
    newHead.y += moveDirection.y * 2;

    if (newHead.x < 0 || newHead.x >= screenWidth || newHead.y < 0 || newHead.y >= screenHeight)
    {
        return false;
    }

    snakeBody.insert(snakeBody.begin(), newHead);

    if (snakeBody.size() > length)
    {
        snakeBody.pop_back();
    }

    return true;
}

bool SnakeGame::drawResult()
{
    if (millis() - resultPrintStart < resultPrintTime)
    {
        u8g.firstPage();
        int x = 20;
        int y = screenHeight / 2 + 15;
        char *message;
        u8g.setFont(u8g_font_9x15);
        if (state == GameState::WIN_MESSAGE)
        {
            message = "You Win!";
        }
        else if (state == GameState::LOSE_MESSAGE)
        {
            message = "You Lose!";
        }
        else if (state == GameState::EXIT_MESSAGE)
        {
            message = "Game Exited!";
        }
        else
        {
            return false; // Should not happen
        }
        u8g.drawStr(x, y, message);
        return true;
    }
    else
    {
        return false;
    }
}

GameResultType SnakeGame::stateToResultType(GameState state) const
{
    switch (state)
    {
    case GameState::WIN_MESSAGE:
        return GameResultType::WIN;
    case GameState::LOSE_MESSAGE:
        return GameResultType::LOSE;
    case GameState::EXIT_MESSAGE:
        return GameResultType::PAUSED;
    default:
        break;
    }
}
