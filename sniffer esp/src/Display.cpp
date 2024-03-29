#include "Display.h"

Display::Display() {
    // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
    // The pins for I2C are defined by the Wire-library.
    // On an arduino UNO:       A4(SDA), A5(SCL)
    this->screen = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    this->isOnVal = true;
    this->scheduledTurnOff = NULL_TIME;
    this->temp = "";
    this->runTime = 0;
    this->page = PAGE_TEMP;
}

void Display::setup() {
  if (!screen.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed!");
  } else {
    Serial.println("SSD1306 allocation succes!");
  }
  screen.clearDisplay();
}

void Display::loop() {
    if (scheduledTurnOff != NULL_TIME) {
        if (scheduledTurnOff < millis()) {
            turnOff();
            scheduledTurnOff = NULL_TIME;
        }
        
    }
}

bool Display::isOn() {
    return this->isOnVal;
}

void Display::turnOn() {
    this->isOnVal = true;
    screen.ssd1306_command(SSD1306_DISPLAYON);
}

void Display::turnOff() {
    this->isOnVal = false;
    this->scheduledTurnOff = NULL_TIME;
    this->page = PAGE_TEMP;
    screen.ssd1306_command(SSD1306_DISPLAYOFF);
}

void Display::scheduleTurnOff(unsigned int delay) {
    this->scheduledTurnOff = millis() + delay;
}

void Display::cancelTurnOff() {
    this->scheduledTurnOff = NULL_TIME;
}

#define LOGO_HEIGHT 32
#define LOGO_WIDTH 32

static const unsigned char PROGMEM logo_bmw[] = {
    0x00, 0x0f, 0xe0, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0xff, 0xff, 0x00, 0x03, 0xe0, 0x07, 0xc0,
    0x07, 0x80, 0x01, 0xe0, 0x0e, 0x00, 0x00, 0x70, 0x1c, 0x00, 0x00, 0x30, 0x18, 0x0f, 0x00, 0x38,
    0x38, 0x3f, 0x00, 0x1c, 0x30, 0x7f, 0x00, 0x0c, 0x70, 0xff, 0x00, 0x0e, 0x60, 0xff, 0x00, 0x06,
    0xe1, 0xff, 0x00, 0x06, 0xe1, 0xff, 0x00, 0x07, 0xe1, 0xff, 0x00, 0x07, 0xe1, 0xff, 0x00, 0x07,
    0xe0, 0x00, 0xff, 0x87, 0xe0, 0x00, 0xff, 0x87, 0xe0, 0x00, 0xff, 0x87, 0x60, 0x00, 0xff, 0x86,
    0x60, 0x00, 0xff, 0x06, 0x70, 0x00, 0xfe, 0x0e, 0x30, 0x00, 0xfe, 0x0c, 0x38, 0x00, 0xf8, 0x1c,
    0x18, 0x00, 0xf0, 0x38, 0x1c, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0xe0, 0x07, 0x80, 0x01, 0xc0,
    0x03, 0xe0, 0x0f, 0x80, 0x00, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x07, 0xe0, 0x00};

#define chWidth 18
#define chHeight 24

void Display::drawTemp(String temp) {
    screen.clearDisplay();
    screen.setCursor(0, 0);
    screen.cp437(true);
    screen.setTextColor(WHITE);
    screen.setTextSize(3);

    unsigned int len = temp.length();
    screen.setCursor(
        (SCREEN_WIDTH - (len + 1) * chWidth - chWidth / 2) / 2,
        (SCREEN_HEIGHT - chHeight) / 2
    );

    screen.print(temp);
    screen.write(248);
    screen.print("C");
    screen.display();
}

void Display::drawRunTime(unsigned long time) {
    screen.clearDisplay();
    screen.setCursor(0, 0);
    screen.cp437(true);
    screen.setTextColor(WHITE);
    screen.setTextSize(3);

    String hours = String(time / 3600000);

    unsigned int len = hours.length();
    screen.setCursor(
        (SCREEN_WIDTH - (len + 1) * chWidth - chWidth / 2) / 2,
        (SCREEN_HEIGHT - chHeight) / 2
    );

    screen.print(hours);
    screen.print("h");
    screen.display();
}

void Display::drawLogo() {
    screen.clearDisplay();
    screen.drawBitmap(
        (screen.width() - LOGO_WIDTH) / 2,
        (screen.height() - LOGO_HEIGHT) / 2,
        logo_bmw,
        LOGO_WIDTH,
        LOGO_HEIGHT,
        1
    );
    screen.display();
}


void Display::draw() {
    Serial.println("draw");
    if (this->page == PAGE_TEMP) {
        drawTemp(this->temp);
    } else if (this->page == PAGE_RUN_TIME) {
        drawRunTime(this->runTime);
    } 
}

void Display::setTemp(String temp) {
    this->temp = temp;
}

void Display::setRunTime(unsigned long time) {
    this->runTime = time;
}

void Display::nextPage() {
    if (this->page == PAGE_TEMP) {
        this->page = PAGE_RUN_TIME;
    } else {
        this->page = PAGE_TEMP;
    }
}
