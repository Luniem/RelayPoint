#include "driver/gpio.h"
#include "esp_log.h"

#include "mqtt_handler.h"
#include "wifi_handler.h"
#include "status_led.h"
#include "button_handler.h"
#include "temperature_handler.h"
#include "led_strip_handler.h"

static const char *TAG = "Main";

void initializeComponents() {
    ESP_LOGI(TAG, "Initializing components...");
    initStatusLED();
    initLeftButtonHandler(toggleStatusLED);

    initTemperatureHandler();
    initLEDStrip();
}

void app_main(void)
{
    // install GPIO ISR service - this should be done only once
    gpio_install_isr_service(0);

    startupWiFi();
    startupMQTT(initializeComponents);
}