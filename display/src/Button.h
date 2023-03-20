#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"
#include "Utils.h"

class Button {
private:
    int PIN;
    unsigned long lastPressed;
    runable clickListener;
    runable longClickListener;
public:
    Button(int pin);

    void loop();
    void setOnClickListener(runable listener);
    void setOnLongClickListener(runable listener);
};

#endif