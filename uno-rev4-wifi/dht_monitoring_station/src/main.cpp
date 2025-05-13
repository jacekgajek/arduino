#include <Arduino.h>
#include <DHT.h>
#include <RTC.h>
#include <MySegDisplay.h>
#include <Clock.h>
#include <Servo.h>

#define PIN_BUTTON PIN_D1

#define LED_DIGIT_1 PIN_D2
#define LED_DIGIT_2 PIN_D4
#define LED_DIGIT_3 PIN_D5
#define LED_DIGIT_4 PIN_D3

#define SHR_RCLK PIN_D10
#define PIN_OE PIN_D8

// #define PIN_SERVO PIN_D9

MySegDisplay display;
DHT dht(PIN_D0, DHT22);
Clock myClock;
// Servo servo;

void triggerButton();
void initWifi();

void setup() {
    pin_size_t ledPins[] ={ LED_DIGIT_1, LED_DIGIT_2, LED_DIGIT_3, LED_DIGIT_4 } ;

    Serial.begin(9600);
    while (!Serial) {
        ;
    }
    Serial.println("Hello Serial!");
    display.begin(ledPins, SHR_RCLK, PIN_OE);
    pinMode(PIN_BUTTON, INPUT_PULLUP);

    Serial.println("Connecting to WiFi...");
    initWifi();

    RTC.begin();
    dht.begin();
    // servo.attach(PIN_SERVO);
    // servo.write(0);
    Serial.println("Initializing clock...");
    myClock.setTimeZone(2);
    myClock.begin();
    if (myClock.update()) {
        Serial.print("Clock initialized. Current time: ");
        Serial.println(myClock.getFormattedTime());
    } else {
        Serial.println("Failed to update clock.");
    }
    attachInterrupt(PIN_BUTTON, triggerButton, RISING);
}

enum DisplayState {
    TIME = 0,
    TEMPERATURE = 1,
    HUMIDITY = 2,
};

DisplayState state = TIME;
int lastDisplayChange = 0;
int lastDhtReading = 0;
int lastClockUpdate = 0;

int dhtReadingPeriod = 2000;
int clockUpdatePeriod = 36000;
int humidity = 0;
int temperature = 0;
// int angle = 0;
bool run = true;

int updateIntervalMs = 2000;
void loop()
{
    if (lastDhtReading == 0 || millis() - lastDhtReading > dhtReadingPeriod) {
        lastDhtReading = millis();
        temperature = static_cast<int>(round(dht.readTemperature()));
        humidity = static_cast<int>(round(dht.readHumidity()));
    }
    if (lastClockUpdate == 0 || millis() - lastClockUpdate > clockUpdatePeriod) {
        lastClockUpdate = millis();
        if (!myClock.update()) {
            Serial.println("Failed to update clock.");
        }
    }
    // angle = (angle + 1) % 180;
        // servo.write(angle+=1);
    if (millis() - lastDisplayChange >= updateIntervalMs)
    {
        if (run) {
            state = static_cast<DisplayState>((state + 1) % 3);
        }

        lastDisplayChange = millis();
        int data = 0;
        int dot = 0;
        int symbol = 0;

        switch (state)
        {
        case TIME: {
            RTCTime time = myClock.getTime();
            data = time.getHour() * 100 + time.getMinutes();
            dot = 2;
            symbol = 0;
            break;
        }
        case HUMIDITY:
            data = humidity;
            symbol = 0b00010000;
            break;
        case TEMPERATURE:
            data = temperature;
            symbol = 0b10000000;
            break;
        default:
            break;
        }
        display.setNumber(data, dot);

        if (symbol > 0) {
            display.setGlyph(0, symbol);
        }
    }
    display.refresh();
}

void triggerButton()
{
    run = !run;
    if (run) {
        lastDisplayChange = 0;
    }
}
