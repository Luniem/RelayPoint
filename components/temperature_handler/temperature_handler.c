#include "temperature_handler.h"

#include "esp_log.h"
#include "mqtt_handler.h"
#include "math.h"
#include "driver/i2c.h"

#define TEMPERATURE_SENSOR_I2C_ADDRESS 0x70
#define I2C_SDA_GPIO 5
#define I2C_SCL_GPIO 6
#define TEMP_MEASUREMENT_PERIOD_MS 10000

static const char *TAG = "TempHandler";

void wakeUpSensor() {
    // values read from the datasheet
    uint8_t buf[32] =  {0x35, 0x17};
    ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_NUM_0, TEMPERATURE_SENSOR_I2C_ADDRESS, buf, 2, pdMS_TO_TICKS(50)));
}

void sleepSensor() {
    // values read from the datasheet
    uint8_t buf[32] =  {0xB0, 0x98};
    ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_NUM_0, TEMPERATURE_SENSOR_I2C_ADDRESS, buf, 2, pdMS_TO_TICKS(50)));
}

void readAndWriteTemperatureAndHumidity() {
    uint8_t data[6];
    uint8_t buf[2] = {0x58, 0xE0};

    // tell sensor to read temperature and humidity - with humidty first
    ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_NUM_0, 0x70, buf, 2, pdMS_TO_TICKS(50)));

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(50));
        esp_err_t err = i2c_master_read_from_device(I2C_NUM_0, 0x70, data, 6, pdMS_TO_TICKS(50));

        if (err == ESP_OK) {
            break;
        }else if (err == ESP_FAIL) {
            ESP_LOGI(TAG, "I2C read failed, retrying...");
        }
    }

    uint16_t rh = data[0] << 8 | data[1];
    uint16_t temp = data[3] << 8 | data[4];

    // TODO: use the CRC to check the data integrity

    // calculate relative humidity and temperature like described in the datasheet
    float_t calc_rhd = (float)rh / (1 << 16) * (float)100;
    float_t calc_temp = (float)temp / (1 << 16) * (float)175 - (float)45;

    ESP_LOGI(TAG, "Read Relative Humidity: %.2f %%", calc_rhd);
    ESP_LOGI(TAG, "Read Temperature: %.2f °C", calc_temp);

    char tempPayload[16];
    char humidityPayload[16];
    snprintf(tempPayload, sizeof(tempPayload), "%.2f", calc_temp);
    snprintf(humidityPayload, sizeof(humidityPayload), "%.2f", calc_rhd);
    mqttPublishText(MQTT_TEMPERATURE_TOPIC, tempPayload);
    mqttPublishText(MQTT_HUMIDITY_TOPIC, humidityPayload);
}

static void temperatureTask(void *arg) {
    while (true) {
        wakeUpSensor();
        vTaskDelay(pdMS_TO_TICKS(1)); // give the sensor time to wake up
        readAndWriteTemperatureAndHumidity();
        sleepSensor();

        // delay before the next measurement
        vTaskDelay(pdMS_TO_TICKS(TEMP_MEASUREMENT_PERIOD_MS));
    }
}

void initTemperatureHandler() {
   i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_GPIO,
        .scl_io_num = I2C_SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0));

    // read sensor periodically in a separate task
    xTaskCreatePinnedToCore(temperatureTask, "temperatureTask", 2048, NULL, 5, NULL, 0);
}

