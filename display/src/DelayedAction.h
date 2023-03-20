#ifndef DELLAYED_ACTION_H
#define DELLAYED_ACTION_H

#include <Arduino.h>
#include <Utils.h>

const int NULL_TIME = 0;

class DelayedAction {
private:
    unsigned long startTime;
    runable action;
public:
    DelayedAction();

    void loop();

    void runDelayed(int delay, runable r);
    void dismis();
};


DelayedAction::DelayedAction() {
    this->startTime = NULL_TIME;
    this->action = []() {};
}

void DelayedAction::loop() {
    if (startTime != NULL_TIME) {
        if (startTime <= millis()) {
            action();
            startTime = NULL_TIME;
        }
    }
}

void DelayedAction::runDelayed(int delay, runable r) {
    this->startTime = millis() + delay;
    this->action = r;
}

void DelayedAction::dismis() {
    this->startTime = NULL_TIME;
}

#endif