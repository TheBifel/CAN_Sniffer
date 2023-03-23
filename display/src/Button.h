#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"
#include "Utils.h"

#define SHORT_CLICK 0
#define LONG_CLICK 1

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