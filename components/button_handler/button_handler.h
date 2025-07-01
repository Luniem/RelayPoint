#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

typedef void (*button_callback_t)(void);

void initLeftButtonHandler(button_callback_t callback);
void initRightButtonHandler(button_callback_t callback);

#endif