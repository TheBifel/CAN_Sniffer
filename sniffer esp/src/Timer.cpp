#include "Timer.h"

Timer::Timer() {
    this->time = 0;
    this->lastUpdate = 0;
    readFromEEPROM();
}

void Timer::loop() {
    readFromEEPROM();
    Serial.println("read - " + String(time));
    unsigned long now = millis();
    time += now - lastUpdate;   
    lastUpdate = now;
    saveToEEPROM();
}

void Timer::reset() {
    time = 0;
    saveToEEPROM();
}

void Timer::saveToEEPROM() {
    EEPROM.begin(E_SIZE);  // Initialize the EEPROM
    EEPROM.put(E_ADDR, time);
    EEPROM.commit();  // Commit the changes to EEPROM
}

void Timer::readFromEEPROM() {
    EEPROM.begin(E_SIZE);  // Initialize the EEPROM
    EEPROM.get(E_ADDR, time);
    EEPROM.end();  // Release the EEPROM
}
