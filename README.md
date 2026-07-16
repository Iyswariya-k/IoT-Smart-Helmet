# IoT Smart Helmet with GPS and Injury Detection for Coal-Mine Safety

## Overview

The IoT Smart Helmet is an ESP32-based embedded system designed to improve coal-mine worker safety by continuously monitoring environmental conditions, worker health, GPS location, and fall detection. The system transmits real-time sensor data to the Blynk IoT platform for remote monitoring and emergency alerts.

---

## Objective

- Enhance the safety of coal-mine workers through continuous monitoring.
- Detect hazardous gas levels and abnormal environmental conditions.
- Monitor worker health using heart rate and SpO₂ sensors.
- Detect falls or injuries using an accelerometer.
- Track the worker's real-time GPS location.
- Send emergency alerts through the Blynk IoT platform.

---

## Features

- Gas Detection (MQ2 & MQ135)
- Temperature & Humidity Monitoring (DHT11)
- Heart Rate & SpO₂ Monitoring (MAX30100)
- GPS Live Location Tracking
- Fall Detection using ADXL345
- Real-Time Monitoring using Blynk IoT
- Emergency Alert Generation

---

## Hardware Components

- ESP32 Development Board
- MQ2 Gas Sensor
- MQ135 Gas Sensor
- DHT11 Temperature & Humidity Sensor
- ADXL345 Accelerometer
- MAX30100 Pulse Oximeter Sensor
- NEO-6M GPS Module
- Breadboard
- Jumper Wires

---

## Software Used

- Arduino IDE
- Embedded C
- Blynk IoT Platform

---

## Communication Protocols

| Device | Protocol |
|---------|----------|
| ADXL345 | I2C |
| MAX30100 | I2C |
| GPS (NEO-6M) | UART |
| MQ2 | ADC |
| MQ135 | ADC |
| DHT11 | Digital GPIO |

---

## Working Principle

The ESP32 continuously collects data from the MQ2, MQ135, DHT11, ADXL345, MAX30100, and GPS modules. The controller processes the sensor values in real time to detect hazardous gases, monitor temperature and humidity, measure heart rate and SpO₂, detect falls, and track GPS location. The processed data is sent to the Blynk IoT platform for remote monitoring. If a fall is detected, the system immediately generates an emergency alert and updates the worker's status and GPS location.

---

## Block Diagram

```text
                     +----------------------+
                     |       ESP32          |
                     +----------------------+
        |          |          |         |         |         |
        |          |          |         |         |         |
     MQ2 Gas    MQ135 Gas   DHT11   ADXL345   MAX30100   GPS
      Sensor      Sensor    Temp &  Fall       Heart      NEO-6M
                            Humidity Detection  Rate &
                                                  SpO₂
                                  |
                                  |
                           Blynk IoT Cloud
                                  |
                                  |
                          Mobile Application
```

---

## Circuit Diagram

```text
                    +------------------+
                    |      ESP32       |
                    +------------------+
                     |  |  |  |  |  |
                     |  |  |  |  |  +----------------------+
                     |  |  |  |  |                         |
                     |  |  |  |  |                     GPS Module
                     |  |  |  |  |
                     |  |  |  |  +------------ MAX30100
                     |  |  |  |
                     |  |  |  +--------------- ADXL345
                     |  |  |
                     |  |  +------------------ DHT11
                     |  |
                     |  +--------------------- MQ135
                     |
                     +------------------------ MQ2
```

---

## Blynk Dashboard

The dashboard displays:

- Temperature
- Humidity
- MQ2 Gas Level
- MQ135 Gas Level
- Heart Rate
- SpO₂
- Fall Detection Status
- GPS Latitude
- GPS Longitude
- Google Maps Location Link

---

## Results

- Successfully monitored environmental conditions in real time.
- Detected hazardous gas levels.
- Measured heart rate and SpO₂.
- Detected falls using the ADXL345 accelerometer.
- Displayed GPS location on Google Maps.
- Sent real-time data to the Blynk IoT dashboard.

---

## Applications

- Coal Mine Safety
- Underground Worker Monitoring
- Industrial Worker Safety
- Hazardous Environment Monitoring

---

## Future Scope

- GSM-based SMS alerts
- Battery monitoring system
- Helmet removal detection
- Cloud database integration
- Mobile application development
- AI-based health prediction and analytics

---

## Developed By

**Iyswariya K**

Electronics and Communication Engineering Graduate 

Currently pursuing a Diploma in Embedded Systems

Interested in Embedded Systems, Embedded C, IoT, and Firmware Development.
