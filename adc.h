#ifndef ADC_H
#define ADC_H

#include "stm32f4xx.h"
#include <stdint.h>

void pa1_adc_init(void);
void pa1_start_sampling(void);
uint32_t pa1_adc_read(void);

#endif