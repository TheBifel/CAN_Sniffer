# BMW e90 CAN Sniffer Display

This repository contains the firmware for a hobby project that reads BMW E90 CAN bus data and shows it on a small OLED display. The active setup uses one Arduino Nano with an MCP2515 module to sniff CAN frames and one ESP8266 board to drive the UI.

## Active Projects

- `sniffer arduino` reads CAN frames and forwards parsed values over serial.
- `UI ESP` receives those values and renders the OLED UI.

The serial protocol between the two boards is intentionally simple and currently carries coolant temperature and battery voltage values.

## Future Work

The project can be expanded with additional CAN-derived pages and diagnostic data over time.
