#include <mcp_can.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Arduino.h>


const int COOLANT_TEMP_ID = 0x1D0;

#define CAN_INT 2 // Set INT to pin 2
MCP_CAN bus(10);  // Set CS to pin 10
SoftwareSerial display(5, 6); // 5 - TX, 6 - RX

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

void setup() {
  Serial.begin(9600);
  display.begin(9600);

  if (bus.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("MCP2515 Initialized");
  } else {
    Serial.println("MCP2515 Error");
  }

  bus.setMode(MCP_NORMAL);

  pinMode(CAN_INT, INPUT); // Configuring pin for /INT input
}


void loop() {
  if (!digitalRead(CAN_INT)) {
    bus.readMsgBuf(&rxId, &len, rxBuf); // Read data: len = data length, buf = data byte(s)"CT=" + temp
    if (rxId == COOLANT_TEMP_ID) {
      int temp = rxBuf[0] - 48;
      String toSend = "CT=" + String(temp);
      Serial.println(toSend);
      display.println(toSend);
      display.flush();
    }
  }
}
                                                                                                                                                                                                                