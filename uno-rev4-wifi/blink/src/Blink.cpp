#include <Globals.h>


void increaseDelay();

void increaseDelay();

void decreaseDelay();

void printQueue();


// the loop function runs over and over again forever
void loop()
{
    init();
    while (true)
    {

        int now = millis();

        if (now - prevLedBlink >= blinkDelay)
        {
            prevLedBlink = now;
            Serial.print("Current delay: ");
            Serial.print(blinkDelay);
            Serial.println(" ms.");
        }

        int half = blinkDelay / 2;
        int quarter = blinkDelay / 4;
        int part = now - prevLedBlink;
        float ledValue;
        if (part < half)
        {
            if (part < quarter)
            {
                ledValue = 1.0f * part / quarter;
            }
            else
            {
                ledValue = -1.0f * (part - quarter) / quarter + 1.0f;
            }
        }
        else
        {
            ledValue = 0.0f;
        }

        analogWrite(LED, (int)(ledValue * 255));

        if (millis() - prevBeepTime >= beepLen)
        {
            int nextTone;
            if (!beepQueue.isEmpty())
            {
                printQueue();
            }
            if (beepQueue.dequeue(nextTone))
            {
                tone(BUZZER, nextTone);
            }
            else
            {
                noTone(BUZZER);
            }
            prevBeepTime = millis();
        }
    }
}

void increaseDelay()
{
    if (blinkDelay < DELTA)
    {
        blinkDelay = DELTA;
    }
    else
    {
        blinkDelay += DELTA;
    }
    EEPROM.put(0, blinkDelay);
    beepQueue.enqueue(659);
    beepQueue.enqueue(440);
}

void decreaseDelay()
{
    if (blinkDelay > DELTA)
    {
        blinkDelay -= DELTA;
    }
    else if (blinkDelay > 2)
    {
        blinkDelay >>= 1;
    }
    EEPROM.put(0, blinkDelay);
    beepQueue.enqueue(440);
    beepQueue.enqueue(659);
}

void printQueue()
{
    Serial.print("[");
    for (size_t i = 0; i < beepQueue.count(); i++)
    {
        int elem;
        beepQueue.get(i, elem);
        Serial.print(elem);
        Serial.print(" ");
    }
    Serial.println("]");
}
