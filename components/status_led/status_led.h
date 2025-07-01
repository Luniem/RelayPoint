#ifndef STATUS_LED_H
#define STATUS_LED_H

void initStatusLED();
void toggleStatusLED();
void receiveMQTTStatusLEDCommand(const char *data);

#endif