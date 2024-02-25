#include <SoftwareSerial.h>
#include <Button.h>
#include <Display.h>

auto sniffer = SoftwareSerial(5, 6); // 5 - TX, 6 - RX
auto button = Button(A0);
auto display = Display();
boolean isInteracted = false;

void setup() {
  display.setup();
  Serial.begin(9600);
  sniffer.begin(9600);

  button.setOnClickListener([](byte type) {
    display.cancelTurnOff();
    isInteracted = true;
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
    } else if (type == LONG_CLICK_FEEDBACK) {
      if (!display.isOn()) {
        Serial.println("OnLongClickFeedback");
        display.turnOn();
        display.drawLogo();
        delay(50);
        display.turnOff();
      }
    } 
    
  });

  display.drawLogo();
}

void loop() {
  button.loop();
  display.loop();

  // if (sniffer.available()) {
  if (Serial.available()) {
    // String read = sniffer.readStringUntil('\n');
    String read = Serial.readStringUntil('\n');
    Serial.println(read);
    if (read.startsWith("CT")) {
      String data = read.substring(3); // CT=75 -> 75
      data.trim();
      display.drawTemp(data);
      Serial.println("Sycle");
      
      if (!isInteracted) {
        int temp = data.toInt();
        if (temp > 60) {
          display.turnOff();
          isInteracted = true;
        }
      }
      
    }
  }
}