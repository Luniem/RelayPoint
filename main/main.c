#include "driver/gpio.h"

#include "mqtt_handler.h"
#include "wifi_handler.h"
#include "status_led.h"
#include "button_handler.h"
#include "temperature_handler.h"

void app_main(void)
{
    // install GPIO ISR service - this should be done only once
    gpio_install_isr_service(0);

    startupWiFi();
    startupMQTT();

    initStatusLED();
    initLeftButtonHandler(toggleStatusLED);

    initTemperatureHandler();
}