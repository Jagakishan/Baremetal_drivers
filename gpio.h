#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx.h"
#include <stdbool.h>

void led_init(void);
void led_on(void);
void led_off(void);

void button_init(void);
bool button_state(void);

#endif