#include "mqtt_handler.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include <string.h>

#include "status_led.h"

static const char *TAG = "MQTT-Handler";

static esp_mqtt_client_handle_t client = NULL;

// helper function to copy data to a null-terminated string
char *copy_to_cstring(const char *data, int len) {
    char *buf = malloc(len + 1);
    if (!buf) return NULL;
    memcpy(buf, data, len);
    buf[len] = '\0';
    return buf;
}

// TODO: Perhaps we log the connected or disconnected state on the display?
static void mqttEventHandlerCB(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Connected to MQTT broker");

            esp_mqtt_client_subscribe(client, MQTT_STATUS_LED_CMD_TOPIC, 1);
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "Disconnected from MQTT broker");
            break;

        case MQTT_EVENT_DATA:
            // cast void pointer to type
            esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;

            // copy the topic and data to null-terminated strings
            char *topic = copy_to_cstring(event->topic, event->topic_len);
            char *data = copy_to_cstring(event->data, event->data_len);

            // check what topic we received

            // status LED command topic
            if (strcmp(topic, MQTT_STATUS_LED_CMD_TOPIC) == 0) {
                receiveMQTTStatusLEDCommand(data);
            }

            // free the allocated memory for topic and data
            free(topic);
            free(data);

            break;

        default:
            break;
    }
}

void startupMQTT(void) {
    // only provide username and password if they are set in the config
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_MQTT_BROKER_URI,
#ifdef MQTT_USE_AUTH
        .credentials.username = CONFIG_MQTT_USERNAME,
        .credentials.authentication.password = CONFIG_MQTT_PASSWORD
#endif
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqttEventHandlerCB, NULL);
    esp_mqtt_client_start(client);
}

void mqttPublishText(const char *topic, const char *text) {
    if (client) {
        esp_mqtt_client_publish(client, topic, text, 0, 1, 0);
    }
}
