#include "status_led.h"

#include <stdbool.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "mqtt_handler.h"

#define STATUS_LED_GPIO GPIO_NUM_0

static const char *TAG = "Status LED";

#define MQTT_STATUS_LED_TOPIC "status_led/state"

static bool currentStatusLEDState = false; // default state is off

void writeMQTTStatusLED(int statusLEDState) {
    mqttPublishText(MQTT_STATUS_LED_TOPIC, statusLEDState ? "ON" : "OFF");
}

void writeStatusLED(int statusLEDState) {
    currentStatusLEDState = statusLEDState;
    gpio_set_level(STATUS_LED_GPIO, statusLEDState);
    ESP_LOGI(TAG, "Status LED wrote to %s!", statusLEDState ? "ON" : "OFF");
    writeMQTTStatusLED(statusLEDState);
}

void toggleStatusLED() {
    writeStatusLED(!currentStatusLEDState);
}

void initStatusLED() {
    gpio_config_t statusLEDConf = {
        .pin_bit_mask = (1 << STATUS_LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    
    gpio_config(&statusLEDConf);
    
    writeStatusLED(currentStatusLEDState); // Initialize the LED to the default state
}