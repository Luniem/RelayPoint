#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "mqtt_client.h"

#define MQTT_STATUS_LED_TOPIC "status_led/state"
#define MQTT_STATUS_LED_CMD_TOPIC "status_led/set"

void startupMQTT(void);
void mqttPublishText(const char *topic, const char *text);
#endif