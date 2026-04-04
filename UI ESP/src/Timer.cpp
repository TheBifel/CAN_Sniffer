#include "Timer.h"

namespace {
    struct TimerStorage {
        unsigned long magic;
        unsigned long runTimeSeconds;
        unsigned long invertedRunTimeSeconds;
        unsigned long checksum;
    };

    const unsigned long TIMER_STORAGE_MAGIC = 0x54494D52UL;
    const unsigned long TIMER_STORAGE_SALT = 0xA5A55A5AUL;
    const unsigned int E_ADDR_PRIMARY = 0;
    const unsigned int E_ADDR_BACKUP = sizeof(TimerStorage);
    const unsigned int E_SIZE = sizeof(TimerStorage) * 2;
    const unsigned long SAVE_INTERVAL_SECONDS = 15UL * 60UL;

    unsigned long calculateChecksum(const TimerStorage& storage) {
        return storage.magic ^ storage.runTimeSeconds ^ storage.invertedRunTimeSeconds ^ TIMER_STORAGE_SALT;
    }

    bool isValid(const TimerStorage& storage) {
        return storage.magic == TIMER_STORAGE_MAGIC &&
            storage.invertedRunTimeSeconds == ~storage.runTimeSeconds &&
            storage.checksum == calculateChecksum(storage);
    }
}

Timer::Timer() {
    this->runTimeSeconds = 0;
    this->lastTickMillis = 0;
    this->bufferedMillis = 0;
    this->unsavedSeconds = 0;
}

void Timer::setup() {
    readFromEEPROM();
    this->lastTickMillis = millis();
}

void Timer::loop() {
    unsigned long now = millis();
    bufferedMillis += now - lastTickMillis;
    lastTickMillis = now;

    if (bufferedMillis < 1000UL) {
        return;
    }

    unsigned long elapsedSeconds = bufferedMillis / 1000UL;
    bufferedMillis %= 1000UL;

    runTimeSeconds += elapsedSeconds;
    unsavedSeconds += elapsedSeconds;

    if (unsavedSeconds >= SAVE_INTERVAL_SECONDS) {
        saveToEEPROM();
        unsavedSeconds = 0;
    }
}

void Timer::reset() {
    runTimeSeconds = 0;
    bufferedMillis = 0;
    unsavedSeconds = 0;
    lastTickMillis = millis();
    saveToEEPROM();
}

void Timer::saveToEEPROM() {
    TimerStorage storage;
    storage.magic = TIMER_STORAGE_MAGIC;
    storage.runTimeSeconds = runTimeSeconds;
    storage.invertedRunTimeSeconds = ~runTimeSeconds;
    storage.checksum = calculateChecksum(storage);

    EEPROM.begin(E_SIZE);
    EEPROM.put(E_ADDR_PRIMARY, storage);
    EEPROM.put(E_ADDR_BACKUP, storage);
    EEPROM.commit();
    EEPROM.end();
}

void Timer::readFromEEPROM() {
    TimerStorage primary;
    TimerStorage backup;
    unsigned long legacyRunTimeMillis = 0xFFFFFFFFUL;

    EEPROM.begin(E_SIZE);
    EEPROM.get(E_ADDR_PRIMARY, primary);
    EEPROM.get(E_ADDR_BACKUP, backup);
    EEPROM.get(E_ADDR_PRIMARY, legacyRunTimeMillis);
    EEPROM.end();

    if (isValid(primary) && isValid(backup)) {
        runTimeSeconds = max(primary.runTimeSeconds, backup.runTimeSeconds);
    } else if (isValid(primary)) {
        runTimeSeconds = primary.runTimeSeconds;
    } else if (isValid(backup)) {
        runTimeSeconds = backup.runTimeSeconds;
    } else if (legacyRunTimeMillis != 0xFFFFFFFFUL && legacyRunTimeMillis > 0UL) {
        runTimeSeconds = legacyRunTimeMillis / 1000UL;
        saveToEEPROM();
    } else {
        runTimeSeconds = 0;
    }
}
