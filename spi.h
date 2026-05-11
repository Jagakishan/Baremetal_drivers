#ifndef SPI_H
#define SPI_H

#include "stm32f4xx.h"
#include <stdint.h>

void spi_init(void);
void spi1_transmit(uint8_t* data, int size);
void spi1_receive(uint8_t* data, int size);
void pa4_slave_enable(void);
void pa4_slave_disable(void);

#endif