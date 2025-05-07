#include <Arduino.h>
#include <Arduino_LED_Matrix.h>
#include <DHT.h>

#define LED 6
#define BUZZER 7
#define BUTTON_UP 3

#define DHT_DATA 4

enum Mode {
  TEMPERATURE, HUMIDITY
};

DHT dht(DHT_DATA, DHT22);
ArduinoLEDMatrix matrix;
Mode mode = TEMPERATURE; 
float humidity, temperature;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(DHT_DATA, INPUT);

  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Hello setup!");

  dht.begin();
  matrix.begin();
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(false);
  if (isnan(humidity)) {
    Serial.println("Failed to read humidity.");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }
  if (isnan(temperature)) {
    Serial.println("Failed to read temperature.");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
  }
  delay(2000);

}
