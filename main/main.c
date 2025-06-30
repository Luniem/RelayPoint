#include <stdio.h>

#include "mqtt_handler.h"
#include "wifi_handler.h"
#include "status_led.h"

void app_main(void)
{
    startupWiFi();
    startupMQTT();
    initStatusLED();

    while (1)
    {
        toggleStatusLED();
        vTaskDelay(pdMS_TO_TICKS(1000)); // Toggle every second
    }
    
}