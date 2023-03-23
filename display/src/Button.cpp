#include "Button.h"

Button::Button(int pin){
    pinMode(pin, INPUT_PULLUP);
    this->PIN = pin;
    this->lastPressed = millis();
}

void Button::setOnClickListener(supplier listener) {
    this->clickListener = listener;
}

int state = HIGH;

void Button::loop() {    
    int newState = digitalRead(PIN);
    if (newState == state) return;
    state = newState;
    if (state == LOW) {
        lastPressed = millis();
    } else {
        unsigned long now = millis();
        if (now - lastPressed < 300) {
            clickListener(SHORT_CLICK);
        } else {
            clickListener(LONG_CLICK);
        }
    }
}
