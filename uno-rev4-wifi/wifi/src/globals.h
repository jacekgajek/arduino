#pragma once
#include <WiFiS3.h>
#include <DHT.h>
#include <Blinker.h>
#include <Buzzer.h>

#define LED_PIN 6
#define BUZZER_PIN 7
#define BUTTON_UP_PIN 3
#define BUTTON_DOWN_PIN 2

#define DHT_DATA_PIN 4

#define READ_PERIOD 2000

const long TIME_ZONE_OFFSET_HOURS = 2;

extern WiFiServer server;
extern DHT dht;
extern Blinker blinker;
extern Buzzer buzzer;
extern Clock myClock;

extern volatile bool printInfo;
extern volatile bool alarm;
