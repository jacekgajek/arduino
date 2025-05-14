
#include <Arduino.h>
#include <DHT.h>
#include <Keypad.h>

#define PIN_LED D5
#define PIN_DHT22 D2

DHT dht(PIN_DHT22, DHT22);

int lastUpdate = 0;
float humidity = 0;
float temperature = 0;


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {3, 4, 5, 6};
byte colPins[COLS] = {7, 8, 9, 10};

Keypad keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
void setup() {
    Serial.begin(9600);

    while (!Serial) {
    }

    for (size_t i = 0; i < ROWS; i++)
    {
        pinMode(rowPins[i], OUTPUT);
        pinMode(colPins[i], INPUT);
    }
    
    Serial.println("Hello Serial!");

    dht.begin();
    pinMode(PIN_LED, OUTPUT);
}

void loop() {
    while (true) {
        if (lastUpdate == 0 || millis() - lastUpdate > 6000) {
            humidity = dht.readHumidity();
            temperature = dht.readTemperature();
        Serial.println("Loop!");

            if (isnan(humidity) || isnan(temperature)) {
                Serial.println("Failed to read from DHT sensor!");
            } else {
                Serial.print("Humidity: ");
                Serial.print(humidity);
                Serial.print("%, Temperature: ");
                Serial.print(temperature);
                Serial.println("°C");
            }
            lastUpdate = millis();
        }

        char key = keypad.getKey();
        if (key != NO_KEY) {
            Serial.println(key);
        }
        byte state = 0;
        for (byte i = 0; i < 4; i++)
        {
            auto pinState = digitalRead(rowPins[i]);
            if (pinState == HIGH) {
                Serial.print("1");
            } else {
                Serial.print("0");
            }
        }
        for (byte i = 0; i < 4; i++)
        {
            auto pinState = digitalRead(colPins[i]);
            if (pinState == HIGH) {
                Serial.print("1");
            } else {
                Serial.print("0");
            }
        }
        
        Serial.println();
        delay(200);
        // digitalWrite(PIN_LED, HIGH);
        // delay(1000);
        // digitalWrite(PIN_LED, LOW);
    }
}
