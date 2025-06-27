#include <stdio.h>

#include "mqtt_handler.h"
#include "wifi_handler.h"

void app_main(void)
{
    startupWiFi();
    startupMQTT();
}