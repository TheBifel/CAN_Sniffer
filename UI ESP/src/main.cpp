#include <SoftwareSerial.h>
#include <Button.h>
#include <Display.h>
#include <Timer.h>

auto sniffer = SoftwareSerial(15, 13); // RX, TX
auto button = Button(A0);
auto display = Display();
auto timer = Timer();
boolean isInteracted = false;
unsigned long lastRuntimeHours = 0;

void setup() {
  Serial.begin(9600);
  display.setup();
  sniffer.begin(9600);
  timer.setup();
  display.setRunTime(timer.runTimeSeconds);
  lastRuntimeHours = timer.runTimeSeconds / 3600UL;

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
          display.setRunTime(timer.runTimeSeconds);
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
  timer.loop();

  unsigned long runtimeHours = timer.runTimeSeconds / 3600UL;
  if (runtimeHours != lastRuntimeHours) {
    lastRuntimeHours = runtimeHours;
    display.setRunTime(timer.runTimeSeconds);
    if (display.isOn() && display.page == PAGE_RUN_TIME) {
      display.draw();
    }
  }

  if (sniffer.available()) {
  // if (Serial.available()) {
    String read = sniffer.readStringUntil('\n');
    // String read = Serial.readStringUntil('\n');
    Serial.println(read);
    if (read.startsWith("CT=")) {
      String data = read.substring(3); // CT=75 -> 75
      data.trim();
      display.setTemp(data);
      display.draw();
      int temp = data.toInt();
      Serial.println("Sycle");
      
      if (temp >= 98) {
        display.turnOn();
        isInteracted = true;
      }

      if (!isInteracted) {
        if (temp > 60) {
          display.turnOff();
          isInteracted = true;
        }
      }
      
    } else if (read.startsWith("BV=")) {
      String data = read.substring(3); // BV=12.2 -> 12.2
      data.trim();
      display.setBatteryVoltage(data);
      if (display.isOn() && display.page == PAGE_BATTERY) {
        display.draw();
      }
    }
  }

  delay(50);
}

// CT=25
// CT=75
// CT=94
// CT=90
