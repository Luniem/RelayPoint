#include "button_handler.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#define LEFT_BUTTON_GPIO GPIO_NUM_9  // Change to your actual GPIO number
#define RIGHT_BUTTON_GPIO GPIO_NUM_2 // Change to your actual GPIO number

static const char *TAG = "ButtonHandler";

static QueueHandle_t gpioLeftEventQueue = NULL;
static button_callback_t leftButtonCallback = NULL;
static button_callback_t rightButtonCallback = NULL;

static void IRAM_ATTR gpioISRHandler(void* arg) {
    uint32_t gpioNum = (uint32_t) arg;
    xQueueSendFromISR(gpioLeftEventQueue, &gpioNum, NULL);
}

static void buttonTask(void* arg) {
    int gpioNum;
    while (1) {
        if (xQueueReceive(gpioLeftEventQueue, &gpioNum, portMAX_DELAY)) {
            // debounce
            vTaskDelay(pdMS_TO_TICKS(50));

            if (gpio_get_level(gpioNum) == 0) { 
                ESP_LOGI(TAG, "Button press detected on GPIO %d", gpioNum);
                if (leftButtonCallback) {
                    leftButtonCallback(); 
                }
            }
        }
    }
}

void initLeftButtonHandler(button_callback_t callback) {
    leftButtonCallback = callback;

    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1 << LEFT_BUTTON_GPIO),
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };
    gpio_config(&io_conf);

    gpioLeftEventQueue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(buttonTask, "buttonTask", 2048, NULL, 10, NULL);

    gpio_isr_handler_add(LEFT_BUTTON_GPIO, gpioISRHandler, (void*) LEFT_BUTTON_GPIO);
}