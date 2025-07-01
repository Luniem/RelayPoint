idf_component_register(SRCS "led_strip_handler.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "led_strip" "mqtt_handler")
