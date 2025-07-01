# RelayPoint
ESP32 firmware for sensor data collection and actuator control via MQTT, integrated with Home Assistant for seamless smart home automation.

## Getting Started

1. Set up a MQTT broker (e.g., Mosquitto).
2. Configure Home Assistant MQTT integration.
3. Customize config for environment (e.g. MQTT) as needed. 
3. Flash the RelayPoint firmware on your ESP32 using ESP-IDF.
5. Add MQTT devices (switches, sensors) in Home Assistant.
6. Build dashboards and automations to interact with your ESP32.

## Setting up config

To get the firmware working, there are a few settings, which will need configuration. To startup the component-configurator, open up a terminal and execute following command: 

`idf.py menuconfig`

In here, navigate to **Component config**.

Following configs need to be overwritten to get the system to work as intended:

- MQTT Broker URI
- Use MQTT authentication
- MQTT Username (if authentication enabled)
- MQTT Password (if authentication enabled)
- WIFI SSID
- WIFI Password
- WIFI maximum retries