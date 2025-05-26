#include <SnakeGame.hpp>

SnakeGame::SnakeGame(LcdShieldJoystick &joystick, U8GLIB &u8g) : joystick(joystick), u8g(u8g)
{
    screenHeight = u8g.getHeight();
    screenWidth = u8g.getWidth();

    state = GameState::RUNNING;
    moveDirection = {1, 0};
}

void SnakeGame::resume()
{
    state = GameState::RUNNING;
    result = {GameResultType::PAUSED, 0};
    lastDebounce = millis();
    lastUpdate = millis();
    resultPrintStart = 0;
    createFood();
}

void SnakeGame::begin()
{
    moveDirection = {1, 0};
    snakeBody.clear();
    vector2d headPosition{screenWidth / 2, screenHeight / 2};
    snakeBody.push_back(headPosition);
    vector2d tailPosition{headPosition.x - 1, headPosition.y };
    snakeBody.push_back(tailPosition);
    length = 2;
    score = 0;
    updateDelay = initialUpdateDelay;

    resume();
}

void SnakeGame::readJoystick() 
{
    auto joy = joystick.read();
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
            state = PAUSE_MESSAGE;
            break;
        default:
            break;
        }
    }
}

bool SnakeGame::gameLoop()
{
    bool running = true;

    readJoystick();

    u8g.firstPage();
    do
    {
        running = running && draw();
    } while (u8g.nextPage());

    return running;
}

bool SnakeGame::draw()
{
    bool running = true;
    if (state != GameState::RUNNING)
    {
        if (resultPrintStart == 0)
        {
            resultPrintStart = millis();
            result.score = length;
            result.result = stateToResultType(state);
        }
        if (!drawResult())
        {
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
                state = GAME_OVER_MESSAGE;
            }
            if (snakeBody.front() == foodPosition)
            {
                length++;
                score++;
                if (length == screenHeight * screenWidth / (snakeThickness * snakeThickness) - 4)
                {
                    for (size_t i = 1; i < length; i++)
                    {
                        snakeBody.pop_back();
                    }
                    length = 1;
                }
                createFood();
            }
        }
        drawFood();
        drawSnake();
    }
    return running;
}

void SnakeGame::drawSnake()
{
    for (auto &&snakePart : snakeBody)
    {
        u8g.drawBox(snakePart.x, snakePart.y, snakeThickness, snakeThickness);
    }
}

void SnakeGame::drawFrame()
{
    u8g.drawHLine(0, 0, screenWidth);
    u8g.drawHLine(0, screenHeight - 1, screenWidth);
    u8g.drawVLine(0, 0, screenHeight);
    u8g.drawVLine(screenWidth - 1, 0, screenHeight);
}

void SnakeGame::createFood()
{
    int w = screenWidth / snakeThickness - 2;
    int h = screenHeight / snakeThickness - 2;
    bool freeSpace[h * w] = { true };

    for (size_t i = 0; i < length; i++)
    {
        freeSpace[snakeBody[i].y * h + snakeBody[i].x] = false;
    }

    int f = random(0, h * w - length);
    int x = 0, y = 0;
    for (; x < h; x++)
    {
        for (; y < w; y++)
        {
            if (freeSpace[x * h + y])
            {
                if (f == 0)
                {
                    goto exit;
                }
                f--;
            }
        }
    }

exit:
    foodPosition = {x + 1, y + 1};
}

void SnakeGame::drawFood()
{
    u8g.drawBox(foodPosition.x, foodPosition.y, snakeThickness, snakeThickness);
}

bool SnakeGame::move()
{
    vector2d newHead(snakeBody.front());
    newHead.x += moveDirection.x * snakeThickness;
    newHead.y += moveDirection.y * snakeThickness;

    if (newHead.x < 0 || newHead.x >= screenWidth || newHead.y < 0 || newHead.y >= screenHeight)
    {
        return false;
    }

    if (std::find(snakeBody.begin() + 1, snakeBody.end(), newHead) != snakeBody.end())
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
        int x = 10;
        int y = screenHeight / 2 - 10;
        char *message;
        u8g.setFont(u8g_font_9x15);
        if (state == GameState::GAME_OVER_MESSAGE)
        {
            message = "Game Over!";
        }
        else if (state == GameState::PAUSE_MESSAGE)
        {
            message = "Game Paused!";
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
    case GameState::GAME_OVER_MESSAGE:
        return GameResultType::GAME_OVER;
    case GameState::PAUSE_MESSAGE:
        return GameResultType::PAUSED;
    default:
        break;
    }
}
