#include <SoftwareSerial.h>
#include <Button.h>
#include <Display.h>

auto sniffer = SoftwareSerial(5, 6); // 5 - TX, 6 - RX
auto button = Button(3);
auto display = Display();

void setup() {
  display.setup();
  Serial.begin(9600);
  sniffer.begin(9600);

  button.setOnClickListener([](byte type) {
    display.cancelTurnOff();
    if (type == SHORT_CLICK) {
      if (display.isOn()) {
        Serial.println("OnClick - turnOff");
        display.turnOff();
      }
      else {
        Serial.println("OnClick - turnOn");
        display.turnOn();
        display.scheduleTurnOff(4000);
      }
    } else if (type == LONG_CLICK) {
      if (display.isOn()) {
        display.turnOff();
        Serial.println("OnLongClick - turnOff");
      } else {
        Serial.println("OnLongClick - turnOn");
        display.turnOn();
      }
    }
  });

  display.drawLogo();
  display.scheduleTurnOff(30000);
}

void loop() {
  button.loop();
  display.loop();

  if (sniffer.available()) {
    String read = sniffer.readStringUntil('\n');
    Serial.println(read);
    if (read.startsWith("CT")) {
      String temp = read.substring(3); // CT=75 -> 75
      temp.trim();
      display.drawTemp(temp);
      Serial.println("Sycle");
    }
  }
}

