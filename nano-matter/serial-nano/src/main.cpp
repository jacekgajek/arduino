#include <Arduino.h>


void setup() {
    Serial.begin(9600);

    while (!Serial) {
    }
    
    Serial.println("Hello Serial from Nano!");
    Serial1.begin(9600);
    Serial.println("Serial1 begin");
    while (!Serial1) {
    }
    Serial.println("Serial1 begin end");
}

void loop() {
    while (true) {
        Serial.println("Loop!");
        delay(100);
    }
}
