# Integration Ideas for RelayPoint (ESP32) with Home Assistant

This document summarizes ideas to integrate the used ESP32 development board features with Home Assistant using MQTT.

## Sensors & Data Collection

- **Temperature- & Humidity-Sensor, Accelerometer & Gyroscope**  
  Publish real-time environment data of temperature, humidity, acceleration and orientation for dashboards and alerts.

- **Potentiometer**  
  Use the potentiometer and send it to Home-Assistant, could be used to dim lights or something. \
  **WARNING** Potentiometer on ESP will trigger a button on certain state.

## Actuators & Controls

- **Buttons**  
  Trigger actions like changing lights or swapping modes.

- **Two Yellow LEDs (Push/Pull configuration)**  
  Use as status indicators for system states, connection status, or alerts. We need to use jumpers for that one, which will deactivate other functions. We could probably use the LED with push-configuration, which will deactivate SD.

- **25x WS2812b RGB LEDs**  
  We could generate a "mood-board" based on temperature and humidity in HA and set the LED's to corresponding colors or lighting-patterns.

- **Servo Control**  
  Control a servo via HA to simulate a Cover, which can be opened, closed or moved to a specific position.

## Communication & Extensions

- **RS-485 Transceiver**  
  Use a industrial sensor or simulate a modbus slave with a RaspberryPi and read some data and transfer it to the Home-Assistant.

- **Micro SD Card Slot**  
  Log sensor data locally and update the logs to the HA periodically. \
  **SD Card will block LED via Jumper!**

- **Display**  
  Connect display and show a message that can be entered in the HA-UI.
---

Realized ideas will be noted in the README.