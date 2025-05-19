
#include <Arduino.h>
#include <DHT.h>

#define PIN_LED_EXT D3
#define PIN_DHT22 D2

DHT dht(PIN_DHT22, DHT22);

int lastUpdate = 0;
float humidity = 0;
float temperature = 0;

void setup() {
    Serial.begin(9600);

    while (!Serial) {
    }
    
    Serial.println("Hello Serial!");
    Serial1.begin(9600);

    dht.begin();
    pinMode(PIN_LED_EXT, OUTPUT);
    pinMode(D4, INPUT);
    pinMode(D5, INPUT);
}


void loop() {
    Serial.println("Entering main loop!");
    while (true) {
        if (lastUpdate == 0 || millis() - lastUpdate > 6000) {
            humidity = dht.readHumidity();
            temperature = dht.readTemperature();

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

        delay(100);
        if (digitalRead(D4) == HIGH) {
            digitalWrite(PIN_LED_EXT, HIGH);
        } 
        if (digitalRead(D5) == HIGH) {
            digitalWrite(PIN_LED_EXT, LOW);
        }
        Serial.println("Reading from Serial1...");

        while(Serial1.available()) {
            char c = Serial1.read();
            Serial.print(c);
        }
        // Uncomment the following lines to blink the LEkk
        // digitalWrite(PIN_LED, HIGH);
        // delay(1000);
        // digitalWrite(PIN_LED, LOW);
        Serial.println("Loop!");
    }
}
