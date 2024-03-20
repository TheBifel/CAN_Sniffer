#include <SoftwareSerial.h>
#include <Button.h>
#include <Display.h>
#include <Timer.h>
#include <EEPROM.h>

auto sniffer = SoftwareSerial(15, 13); // RX, TX
auto button = Button(A0);
auto display = Display();
auto timer = Timer();
boolean isInteracted = false;

void updateTimer() {
  timer.loop();
  display.setRunTime(timer.time);
}

void setup() {
  display.setup();
  Serial.begin(9600);
  sniffer.begin(9600);

  timer1_attachInterrupt(updateTimer);
  timer1_write(3600000);  // ~ 10 second interval
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);

  button.setOnClickListener([](byte type) {
    display.cancelTurnOff();
    isInteracted = true;
    if (type == SHORT_CLICK) {
      if (display.isOn()) {
        Serial.println("OnClick - next");
        display.nextPage();
        display.draw();
      } else {
        Serial.println("OnClick - turnOn");
        display.turnOn();
        display.draw();
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
    } else if (type == LOOONG_CLICK) {
      Serial.println("OnLooongClick");
      if (display.isOn()) {
        if (display.page == PAGE_RUN_TIME) {
          Serial.println("reset timer");
          timer.reset();
          display.setRunTime(timer.time);
          display.draw();
        }
      }
    } else if (type == LONG_CLICK_FEEDBACK) {
      if (!display.isOn()) {
        Serial.println("OnLongClickFeedback");
        display.turnOn();
        display.draw();
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

  if (sniffer.available()) {
  // if (Serial.available()) {
    String read = sniffer.readStringUntil('\n');
    // String read = Serial.readStringUntil('\n');
    Serial.println(read);
    if (read.startsWith("CT")) {
      String data = read.substring(3); // CT=75 -> 75
      data.trim();
      display.setTemp(data);
      display.draw();
      int temp = data.toInt();
      Serial.println("Sycle");
      
      if (temp >= 93) {
        display.turnOn();

      }

      if (!isInteracted) {
        if (temp > 60) {
          display.turnOff();
          isInteracted = true;
        }
      }
      
    }
  }

  delay(50);
}

// CT=25
// CT=75
// CT=94
// CT=90