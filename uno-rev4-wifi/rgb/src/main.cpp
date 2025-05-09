#include <Arduino.h>
#include <RgbLed.h>

#define PIN_LM_32 PIN_A0

#define ADC_VREF_mV    3300.0 // in millivolt
#define ADC_RESOLUTION 4096.0

RgbLed led(PIN_D11, PIN_D10, PIN_D9);

void setup() {
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
    Serial.println("Hello serial!");
    pinMode(PIN_LM_32, PinMode::INPUT);
    led.setColor(0, 0, 0);
}

void loop() {
    int reading = analogRead(PIN_LM_32);
    float milliVolt = reading * ADC_VREF_mV / ADC_RESOLUTION;

    float celcius = milliVolt / 10.0;

    led.fadeToColor(1, 0, 0, 100);
    led.fadeToColor(0, 1, 0, 100);
    led.fadeToColor(0, 0, 1, 100);
}
