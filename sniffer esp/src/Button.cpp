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
    int read = analogRead(PIN);
    int newState;
    if (read > 500) {
        newState = LOW;    
    } else {
        newState = HIGH;
    }

    unsigned long now = millis();
    if (newState == state) {
        if (newState == LOW) {
            if (now - lastPressed >= LONG_CLICK_DURATION) {
                clickListener(LONG_CLICK_FEEDBACK);
            } 
        }
        return;
    }

    state = newState;
    if (state == LOW) {
        lastPressed = millis();
    } else {
        if (now - lastPressed < LONG_CLICK_DURATION) {
            clickListener(SHORT_CLICK);
        } else {
            clickListener(LONG_CLICK);
        }
    }
}
