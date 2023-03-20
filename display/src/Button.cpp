#include "Button.h"

Button::Button(int pin){
    pinMode(pin, INPUT_PULLUP);
    this->PIN = pin;
    this->lastPressed = millis();;
    this->clickListener = []() { Serial.println("OnClick"); };
    this->longClickListener = []() { Serial.println("OnLongClick"); };
}

void Button::setOnClickListener(runable listener) {
    this->clickListener = listener;
}

void Button::setOnLongClickListener(runable listener) {
    this->longClickListener = listener;
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
            clickListener();
        } else {
            longClickListener();
        }
    }
}
