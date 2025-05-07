#pragma once

#include <Arduino.h>
#include <CyclicBuffer.h>
#include <EEPROM.h>

#define LED 6
#define BUZZER 7
#define BUTTON_UP 3
#define BUTTON_DOWN 2
#define BUTTON_POWER 9
#define BEEP_BUFFER_SIZE 100

#define DELTA 200

#define EEPROM_DELAY_LOCATION 0

volatile int blinkDelay = 3000;

int prevBeepTime = 0;
int beepLen = 100;
int prevLedBlink = 0;
bool running = true;
CyclicBuffer beepQueue(BEEP_BUFFER_SIZE);

void init();
void loop();