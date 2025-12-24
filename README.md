# RescueMesh-Flood_Rescue_LoRa_System
🌊 Flood Data Collecting & Rescue-Aided LoRaWAN System

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)  

---

## Overview
This project implements a **low-power, long-range LoRaWAN system** for flood data collection and emergency rescue notifications.  
Using **ESP32 microcontrollers** paired with **AS32-TTL-100 LoRa modules**, the system enables wireless sensor networking, SOS message broadcasting, and remote monitoring with low latency and high reliability.

The firmware is modular and structured, supporting **incremental testing** from GPIO/UART sanity checks to full production-ready emergency node firmware.

---

## Key Features
- **Wireless Communication:** 433MHz LoRa spread spectrum with up to 3 km range (line of sight).  
- **Ultra-Low Power:** Sleep current as low as 1.5 µA; supports wake-on-radio mode.  
- **Robust Transmission:** Automatic relays, point-to-point and broadcast messaging, deduplication, and retransmission.  
- **Configurable Parameters:** UART baud rates (1200–115200 bps), air speeds (0.3–19.2 kbps), transmit power levels, and channels.  
- **Data Security:** Built-in encryption support for sensitive data.  
- **Modular Firmware:** Structured code plan enabling step-by-step testing and integration.

---

## Hardware Requirements
- **ESP32 NodeMCU Development Board (CH340)**  
- **AS32-TTL-100 LoRa Module**  
- **Optional Sensors:** water level, rainfall, or environmental monitoring  
- **Power Supply:** 3.3–5.5 VDC  

### Standard Pinout
| ESP32 Pin | AS32 Pin | Function |
|-----------|----------|---------|
| D17       | RXD      | LoRa RX (ESP32 → AS32) |
| D16       | TXD      | LoRa TX (AS32 → ESP32) |
| D25       | MD0      | Mode select pin 0 |
| D26       | MD1      | Mode select pin 1 |
| D27       | AUX      | Module status indication |
| 3V3       | VCC      | Power supply |
| GND       | GND      | Ground |

> **Note:** All code versions use this pinout for consistency across modules.

---

## Firmware Code Plan
1. **Code 1.0:** ESP32 GPIO & UART Sanity Test  
2. **Code 1.0.2:** AS32 Transparent Test (Factory Settings) – Range Testing  
3. **Code 1.1:** AS32 Parameter Read Code  
4. **Code 1.1.1:** AS32 Parameter Write Code  
5. **Code 1.2:** Point-to-Point Messaging  
6. **Code 1.2.1:** Broadcast & Monitor Mode  
7. **Code 1.3:** RSSI & Link Quality Test  
8. **Code 1.3.1:** Deduplication Test  
9. **Code 1.3.2:** Wake-on-Radio Test  
10. **Code 1.3.3:** Emergency Button Node Firmware  
11. **Code 1.4:** Gateway Aggregation  
12. **Code 1.4.1:** Backend Upload  
13. **Code 1.5:** Production Firmware

> **Important:** Ensure **shared configuration (Code 1.3.3)** is finalized before moving to Codes 1.4 and above.

---

## AS32-TTL-100 Shared Configuration
- **Module Address:** `ADDH = 0x00`, `ADDL = 0x00`  
- **UART / Air Configuration:** `SPEED = 0x1A` (9600 bps, 2.4 kbps air)  
- **Channel:** `CHAN = 0x17` (433 MHz)  
- **Operation Mode & Options:** `OPTION = 0x40` (transparent, push-pull, 250 ms wake, max TX power)  

---

## Project Features & Capabilities
- Transparent broadcasting and monitoring  
- Point-to-point transmission across channels  
- Wake-on-radio for ultra-low power nodes  
- Deduplication of messages for network efficiency  
- Configurable retransmission delays and TTL  
- Real-time RSSI and link quality monitoring  

---

## Getting Started
1. **Clone the Repository**
   ```bash
git clone https://github.com/HiranGeeth/RescueMesh-Flood_Rescue_LoRa_System.git
   cd flood-lorawan

