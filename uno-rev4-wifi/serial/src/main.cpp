#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
    Serial.begin(9600);

    while (!Serial) {
    }
    
    Serial.println("Hello Serial from R4 WiFi!");
    Serial.println("Serial1 begin");

    Serial1.begin(9600); 
    while (!Serial1) {
    }
    Serial.println("Serial1 begin end");
}

void loop() {
    Serial.println("Loop!");
    delay(1000);
}
