#include <includes.h>

WiFiServer server(80);
DHT dht(DHT_DATA_PIN, DHT22);
// Blinker blinker(LED_PIN);
// Buzzer buzzer(BUZZER_PIN, ACTIVE);
Clock myClock;

volatile bool printInfo = true;
volatile bool buzzerState = false;
