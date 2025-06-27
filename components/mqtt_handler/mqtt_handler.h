#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "mqtt_client.h"

void startupMQTT(void);
void mqttPublishText(const char *topic, const char *text);
#endif