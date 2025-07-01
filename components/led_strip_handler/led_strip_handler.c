#include "led_strip_handler.h"

#include "mqtt_handler.h"
#include "led_strip.h"
#include "esp_log.h"

#define LED_STRIP_GPIO 8
#define LED_STRIP_COUNT 25

static led_strip_handle_t ledStrip;

static const char *TAG = "LED Strip Handler";

static uint8_t ledBrightness = 15; // default brightness is maximum
static led_mode_t currentLEDMODE = COLOR_RED; // default state is off

void writeMQTTMode(led_mode_t led_mode_t) {
    // map the enum to a string
    switch (led_mode_t) {
        case COLOR_RED:
            mqttPublishText(MQTT_LED_STRIP_MODE_TOPIC, "RED");
            break;
        case COLOR_GREEN:
            mqttPublishText(MQTT_LED_STRIP_MODE_TOPIC, "GREEN");
            break;
        case COLOR_BLUE:
            mqttPublishText(MQTT_LED_STRIP_MODE_TOPIC, "BLUE");
            break;
        default:
            mqttPublishText(MQTT_LED_STRIP_MODE_TOPIC, "unknown");
            break;
    }
}

void writeMQTTBrightness(uint8_t brightness) {
    char payload[4];
    snprintf(payload, sizeof(payload), "%d", brightness);
    mqttPublishText(MQTT_LED_STRIP_BRIGHTNESS_TOPIC, payload);
}

void writeAllLEDsColor(uint32_t red, uint32_t green, uint32_t blue) {
    for (int i = 0; i < LED_STRIP_COUNT; i++) {
        led_strip_set_pixel(ledStrip, i, red, green, blue);
    }
}

void setLEDMode(led_mode_t mode) {
    currentLEDMODE = mode;
    ESP_LOGI(TAG, "LED Mode set to %d", mode);

    switch (mode) {
        case COLOR_RED:
            writeAllLEDsColor(ledBrightness, 0, 0);
            break;
        case COLOR_GREEN:
            writeAllLEDsColor(0, ledBrightness, 0);
            break;
        case COLOR_BLUE:
            writeAllLEDsColor(0, 0, ledBrightness);
            break;
        default:
            ESP_LOGE(TAG, "Invalid LED mode: %d", mode);
            writeAllLEDsColor(0, 0, 0); // turn off all LEDs if mode is invalid
            break;
    }

    led_strip_refresh(ledStrip);
    writeMQTTMode(mode);
    writeMQTTBrightness(ledBrightness);
}

void initLEDStrip() {
    led_strip_config_t stripConfig = {
        .strip_gpio_num = LED_STRIP_GPIO,
        .max_leds = LED_STRIP_COUNT,
    };

    led_strip_rmt_config_t rmtConfig = {
        .resolution_hz = 10 * 1000 * 1000,
        .flags.with_dma = false,
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&stripConfig, &rmtConfig, &ledStrip));
    led_strip_clear(ledStrip);
    setLEDMode(COLOR_RED);
}

void setLEDBrightness(uint8_t brightness) {
    ledBrightness = brightness;
    ESP_LOGI(TAG, "LED Brightness set to %d", brightness);

    setLEDMode(currentLEDMODE); // update the current mode to apply new brightness
}