#include <SnakeGame.hpp>

SnakeGame::SnakeGame(Joystick &joystick, U8GLIB &u8g) : joystick(joystick), u8g(u8g)
{
    screenHeight = u8g.getHeight();
    screenWidth = u8g.getWidth();

    state = GameState::RUNNING;
    moveDirection = {1, 0};
}

void SnakeGame::resume()
{
    state = GameState::RUNNING;
    previousDirection = moveDirection = {0, 0};
    result = {GameResultType::PAUSED, 0};
    lastDebounce = millis();
    lastUpdate = millis() + 3000;
    resultPrintStart = 0;
}

void SnakeGame::begin()
{
    previousDirection = moveDirection = {1, 0};
    snakeBody.clear();
    vector2d headPosition{screenWidth / 2 / snakeThickness, screenHeight / 2 / snakeThickness};
    snakeBody.push_back(headPosition);
    vector2d tailPosition{headPosition.x - 1, headPosition.y};
    snakeBody.push_back(tailPosition);
    length = 2;
    score = 0;
    updateDelay = initialUpdateDelay;
    moveDirection = {1, 0};
    createFood();

    resume();
}

void SnakeGame::readJoystick()
{
    auto joy = joystick.read();
    if (joy != NONE && millis() - lastDebounce > debounce)
    {
        lastDebounce = millis();
        vector2d newDirection;
        switch (joy)
        {
        case UP:
            newDirection = {0, -1};
            break;
        case DOWN:
            newDirection = {0, 1};
            break;
        case LEFT:
            newDirection = {-1, 0};
            break;
        case RIGHT:
            newDirection = {1, 0};
            break;
        case CENTER:
            state = PAUSE_MESSAGE;
            break;
        default:
            break;
        }
        if (!(newDirection == moveDirection))
        {
            previousDirection = moveDirection;
            moveDirection = newDirection;
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
            result.score = length - 2;
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
        u8g.drawBox(snakePart.x * snakeThickness, snakePart.y * snakeThickness, snakeThickness, snakeThickness);
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
    std::vector<bool> freeSpace(h * w, true);

    for (const auto &snakePart : snakeBody)
    {
        if (snakePart.x >= 0 && snakePart.x < w && snakePart.y >= 0 && snakePart.y < h)
        {
            freeSpace[snakePart.y * w + snakePart.x] = false;
        }
    }

    int f = random(0, h * w - length);
    int x = 0, y = 0;
    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            if (freeSpace[y * w + x])
            {
                if (f == 0)
                {
                    updateDelay = max(50, initialUpdateDelay - score * 10);
                    foodPosition = {x + 1, y + 1};
                    return;
                }
                f--;
            }
        }
    }
}

void SnakeGame::drawFood()
{
    u8g.drawBox(foodPosition.x * snakeThickness, foodPosition.y * snakeThickness, snakeThickness, snakeThickness);
}

bool SnakeGame::move()
{
    if (moveDirection.x == 0 && moveDirection.y == 0)
    {
        return true;
    }
    vector2d newHead(snakeBody.front());
    newHead.x += moveDirection.x;
    newHead.y += moveDirection.y;

    if (newHead.x < 0 || newHead.x >= screenWidth / snakeThickness || newHead.y < 0 ||
        newHead.y >= screenHeight / snakeThickness)
    {
        return false;
    }
    else if (previousDirection.x == -moveDirection.x && previousDirection.y == -moveDirection.y)
    {
        // Snake can reverse if it is straight.
        bool isStraight = true;
        int dx = moveDirection.x;
        int dy = moveDirection.y;
        for (size_t i = 1; i < snakeBody.size(); ++i)
        {
            if (snakeBody[i].x - snakeBody[i - 1].x != dx || snakeBody[i].y - snakeBody[i - 1].y != dy)
            {
                isStraight = false;
                break;
            }
        }

        if (isStraight)
        {
            std::reverse(snakeBody.begin(), snakeBody.end());
            previousDirection = moveDirection;
        }
        else
        {
            return false;
        }
    }
    else if (std::find(snakeBody.begin() + 1, snakeBody.end(), newHead) != snakeBody.end())
    {
        return false;
    }
    else
    {
        snakeBody.insert(snakeBody.begin(), newHead);

        if (snakeBody.size() > length)
        {
            snakeBody.pop_back();
        }
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
