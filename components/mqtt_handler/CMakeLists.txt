idf_component_register(SRCS "mqtt_handler.c"
                    INCLUDE_DIRS "."
                    REQUIRES "mqtt" "status_led" "led_strip_handler")
