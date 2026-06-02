# Agent Notes

## Project Overview
- This repository contains two PlatformIO Arduino firmware projects for a BMW E90 CAN bus display.
- `sniffer arduino` runs on an Arduino Nano with an MCP2515 CAN module. It reads selected CAN frames and forwards parsed values over serial.
- `UI ESP` runs on an ESP8266 module with an SSD1306 OLED. It receives serial values, manages button input, and renders display pages.

## Build Commands
- Build the sniffer firmware with `platformio run -d "sniffer arduino"`.
- Build the ESP UI firmware with `platformio run -d "UI ESP"`.
- Upload commands depend on the connected serial port; do not hard-code a port unless the user provides one.

## Serial Protocol
- The Nano sends newline-terminated key/value lines to the ESP at `9600` baud.
- Current messages are `CT=<coolant temperature>` and `BV=<battery voltage>`.
- Keep new messages simple, line-oriented, and backward-compatible with existing `startsWith()` parsing.

## Coding Guidance
- Keep changes small and consistent with the existing Arduino/C++ style.
- Preserve PlatformIO project boundaries; each firmware has its own `platformio.ini`.
- Avoid dynamic heap-heavy patterns where simple fixed-size values work, especially in frequently called loops.
- Be careful with files and directories that contain spaces in their names; quote paths in shell commands.
- Do not overwrite user changes in the working tree. Check `git status --short` before editing.

## Hardware Notes
- Sniffer CAN interrupt is on Nano pin `2`; MCP2515 chip select is pin `10`.
- Sniffer `SoftwareSerial display(5, 6)` uses pin `5` as TX and pin `6` as RX.
- ESP UI `SoftwareSerial sniffer(13, 12)` uses GPIO `13` as RX and GPIO `12` as TX, equivalent to D1 mini pins `D7` and `D6`.
- The OLED is an I2C SSD1306 display at address `0x3C`, with SDA on GPIO `2` and SCL on GPIO `14`.
- The UI button is read through analog pin `A0`.
