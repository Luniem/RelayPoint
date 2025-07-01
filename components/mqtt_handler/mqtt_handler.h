#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "mqtt_client.h"

#define MQTT_STATUS_LED_TOPIC "status_led/state"
#define MQTT_STATUS_LED_CMD_TOPIC "status_led/set"
#define MQTT_TEMPERATURE_TOPIC "temperature/state"
#define MQTT_HUMIDITY_TOPIC "humidity/state"

#define MQTT_LED_STRIP_MODE_TOPIC "led_strip/state"
#define MQTT_LED_STRIP_BRIGHTNESS_TOPIC "led_strip/brightness"
#define MQTT_LED_STRIP_BRIGHTNESS_CMD_TOPIC "led_strip/brightness/set"

typedef void (*callback_t)(void);

void startupMQTT(callback_t callback);
void mqttPublishText(const char *topic, const char *text);
#endif