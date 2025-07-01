idf_component_register(SRCS "wifi_handler.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_wifi nvs_flash)
