#ifndef LED_STRIP_HANDLER_H
#define LED_STRIP_HANDLER_H

#include <stdint.h>

typedef enum {
    COLOR_RED = 0,
    COLOR_GREEN = 1,
    COLOR_BLUE = 2,
    COLOR_UNKNOWN = 3
} led_mode_t;

void initLEDStrip();
void setLEDMode(led_mode_t mode);
void setLEDBrightness(uint8_t brightness);

#endif