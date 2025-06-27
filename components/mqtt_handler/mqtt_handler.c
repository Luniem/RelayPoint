#include "mqtt_handler.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "MQTT-Handler";

static esp_mqtt_client_handle_t client = NULL;


// TODO: Perhaps we log the connected or disconnected state on the display?
static void mqttEventHandlerCB(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Connected to MQTT broker");
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "Disconnected from MQTT broker");
            break;

        case MQTT_EVENT_DATA:
            const char *str = (const char *)event_data;
            ESP_LOGI(TAG, "Received data on topic %s", str);
            // TODO: Here we handle incoming mqtt messages
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
