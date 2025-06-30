#include "status_led.h"

#include "esp_log.h"
#include "driver/gpio.h"

#define STATUS_LED_GPIO GPIO_NUM_0

static const char *TAG = "Status LED";

static int statusLEDState = 0; // start with LED off

void initStatusLED() {
    gpio_config_t statusLEDConf = {
        .pin_bit_mask = (1 << STATUS_LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    
    gpio_config(&statusLEDConf);
    
    gpio_set_level(STATUS_LED_GPIO, statusLEDState);
}

void toggleStatusLED() {
    statusLEDState = !statusLEDState;
    gpio_set_level(STATUS_LED_GPIO, statusLEDState);
    ESP_LOGI(TAG, "Status LED toggled to %s!", statusLEDState ? "ON" : "OFF");
}