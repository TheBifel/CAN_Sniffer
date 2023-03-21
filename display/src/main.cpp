#include <SoftwareSerial.h>
#include <Button.h>
#include <Display.h>
#include <DelayedAction.h>

auto sniffer = SoftwareSerial(5, 6); // 5 - TX, 6 - RX
auto button = Button(3);
auto display = Display();
auto buttonAction = DelayedAction();

void setup() {
  display.setup();
  Serial.begin(9600);
  sniffer.begin(9600);

  button.setOnClickListener([]() {
    buttonAction.dismis();
    if (display.isOn()) {
        Serial.println("OnClick - turnOff");
        display.turnOff();
    } else {
        Serial.println("OnClick - turnOn");
        display.turnOn();
        buttonAction.runDelayed(2000, []() { 
          Serial.println("action - turnOff");
          display.turnOff();
        });
    } 
  });
  button.setOnLongClickListener([]() {
    buttonAction.dismis();
    if (display.isOn()) {
      display.turnOff();
      Serial.println("OnLongClick - turnOff");
    } else {
      Serial.println("OnLongClick - turnOn");
      display.turnOn();
    } 
  });

  display.drawLogo();
}
void loop() {
  button.loop();
  buttonAction.loop();

  if (sniffer.available()) {
    String read = sniffer.readString();
    if (read.startsWith("CT")) {
      String temp = read.substring(3); // CT=75 -> 75
      display.drawTemp(temp);
    }
  }
}

