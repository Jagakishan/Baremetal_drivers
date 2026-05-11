#ifndef TIM_H
#define TIM_H

#include "stm32f4xx.h"
#include <stdbool.h>

void tim2_init(void);
bool tim2_done(void);

#endif