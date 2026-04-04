#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"
#include "EEPROM.h"

class Timer {
private:
    unsigned long lastTickMillis;
    unsigned long bufferedMillis;
    unsigned long unsavedSeconds;

    void saveToEEPROM();
    void readFromEEPROM();
public:
    unsigned long runTimeSeconds;

    Timer();

    void setup();
    void loop();
    void reset();
};

#endif
