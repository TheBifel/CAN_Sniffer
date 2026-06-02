#ifndef DISPLAY_H
#define DISPLAY_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define OLED_SDA 2
#define OLED_SCL 14

#define NULL_TIME 0


#define PAGE_TEMP 0
#define PAGE_RUN_TIME 1
#define PAGE_BATTERY 2

class Display {
private:
    Adafruit_SSD1306 screen;
    bool isOnVal;
    unsigned long scheduledTurnOff;
    unsigned long runTimeSeconds;
    String temp;
    String batteryVoltage;
    void drawCenteredText(const String& text);
    void drawTemp(String temp);
    void drawRunTime(unsigned long timeSeconds);
    void drawBatteryVoltage(String voltage);

public:
    int page;

    Display();
    void setup();
    void loop();

    bool isOn();
    void turnOn();
    void turnOff();
    void scheduleTurnOff(unsigned int delay);
    void cancelTurnOff();

    void draw();
    void setTemp(String temp);
    void setRunTime(unsigned long timeSeconds);
    void setBatteryVoltage(String voltage);
    void nextPage();
    void drawLogo();
};

#endif
