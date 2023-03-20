#include <mcp_can.h>
#include <SPI.h>
#include <SoftwareSerial.h>

const int COOLANT_TEMP_ID = 0x1D0;

#define CAN_INT 2 // Set INT to pin 2
MCP_CAN bus(10);  // Set CS to pin 10
SoftwareSerial display(5, 6); // 5 - TX, 6 - RX

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

void setup() {
  Serial.begin(115200);
  display.begin(9600);

  if (bus.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  bus.setMode(MCP_NORMAL);
  // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN_INT, INPUT); // Configuring pin for /INT input
}

void loop() {
  // If CAN_INT pin is low, read receive buffer
  if (!digitalRead(CAN_INT)) {
    bus.readMsgBuf(&rxId, &len, rxBuf); // Read data: len = data length, buf = data byte(s)"CT=" + temp
    if (rxId == COOLANT_TEMP_ID) {
      int temp = rxBuf[0] - 48;
      String toSend = "CT=" + temp;
      Serial.print(toSend);
      display.println(toSend);
    }
  }
}
