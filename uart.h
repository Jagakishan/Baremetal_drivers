#ifndef UART_H
#define UART_H

#include "stm32f4xx.h"
#include <stdint.h>

void uart_read(uint8_t* buffer, int numberOfBytes);
void uart_init(void);

#endif