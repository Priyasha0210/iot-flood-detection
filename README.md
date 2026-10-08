# IoT Flood Detection and Early Warning System

An ESP32- and LoRa-based flood monitoring and early warning prototype for remote and flood-prone areas.

## Overview

The system measures the distance between an ultrasonic sensor and the water surface at a transmitter node. The measurement is sent wirelessly over 433 MHz LoRa to a receiver node.

The receiver classifies the incoming measurement into Safe, Moderate, High, and Critical states. It provides local LED/buzzer alerts and uses Wi-Fi to upload measurements and alert-state data to ThingSpeak. Telegram notifications are generated for elevated water-level conditions.

## System Architecture

Ultrasonic Sensor -> ESP32 Transmitter -> 433 MHz LoRa -> ESP32 Receiver -> LEDs/Buzzer
                                                         |
                                                         +-> ThingSpeak
                                                         |
                                                         +-> Telegram

## Hardware

- ESP32 development boards (transmitter and receiver)
- HC-SR04 ultrasonic sensor
- SX1278 LoRa transceiver modules
- Red LED
- Green LED
- Buzzer
- Wi-Fi connection for receiver-side cloud/notification services

## Software / Technologies

- Arduino C/C++
- ESP32
- LoRa
- SPI
- Ultrasonic sensing
- ThingSpeak
- Telegram Bot API
- Wi-Fi

## Flood Severity Classification

The thresholds below are taken from the final receiver code supplied for this project:

| Sensor distance | Status |
|---|---|
| > 12 cm | Safe |
| > 8 cm and <= 12 cm | Moderate |
| > 4 cm and <= 8 cm | High |
| <= 4 cm | Critical |

As the measured distance decreases, the system interprets this as the water level approaching the sensor and increases alert severity.

**Note:** The accompanying research paper contains a different threshold table for its experimental setup. This README documents the thresholds implemented in the final receiver code supplied with the project.

## Data Flow

1. The ultrasonic sensor measures the distance to the water surface.
2. The ESP32 transmitter converts echo time into distance.
3. The measurement is transmitted through LoRa at 433 MHz.
4. The receiver validates the received value.
5. The receiver determines the alert state using predefined thresholds.
6. LEDs and the buzzer provide local warnings.
7. Distance and alert-state values are uploaded to ThingSpeak.
8. Telegram notifications are sent for moderate, high, and critical conditions.

## Results

The accompanying research paper reports:

- Measurement accuracy: 98.5%
- Packet delivery rate: 98%
- Telegram delivery success: 100%
- Average response time: 1.23 seconds
- Cloud update delay: less than 3 seconds
- Prototype deployment cost: INR 2500–3500

See the `results/` directory for project screenshots.

## Research Paper

**LoRa-enabled IoT Flood Monitoring and Early Warning System in Remote Areas**

The accompanying paper documents the system architecture, implementation, experimental evaluation, limitations, and future work.

See `documentation/research-paper.pdf`.

## Security

Do not commit Wi-Fi passwords, Telegram bot tokens, Telegram chat IDs, ThingSpeak write API keys, or other private credentials. The receiver code in this repository uses placeholders; add your real values only in your local copy.

## Future Work

The research paper identifies possible extensions including rainfall, water-flow and soil-moisture sensors; multiple LoRa nodes/LoRaWAN architecture; GPS-based notifications; solar-powered deployment; and enhanced mobile/cloud platforms.

## Authors

M Priyasha  
Hariprasad P Nandan  
Krishnakumar Nair PH  
Dhivvya J P

Department of Electrical and Electronics Engineering, Amrita Vishwa Vidyapeetham, Amritapuri, India.
