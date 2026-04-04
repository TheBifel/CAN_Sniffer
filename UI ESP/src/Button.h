#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"
#include "Utils.h"

#define SHORT_CLICK 0
#define LONG_CLICK 1
#define LOOONG_CLICK 2
#define LONG_CLICK_FEEDBACK 3

#define LONG_CLICK_DURATION 300
#define LOOONG_CLICK_DURATION 5000

class Button {
private:
    int PIN;
    unsigned long lastPressed;
    supplier clickListener;
public:
    Button(int pin);

    void loop();
    void setOnClickListener(supplier listener);
};

#endif