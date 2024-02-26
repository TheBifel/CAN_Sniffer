#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"
#include "Utils.h"
#include "EEPROM.h"

const unsigned int E_ADDR = 0;  // Address in EEPROM to store the delay
const unsigned int E_SIZE = sizeof(unsigned long);

class Timer {
private:
    unsigned long lastUpdate;

    void saveToEEPROM();
    void readFromEEPROM();
public:
    unsigned long time;

    Timer();

    void loop();
    void reset();
};

#endif