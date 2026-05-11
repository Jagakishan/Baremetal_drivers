#ifndef I2C_H
#define I2C_H

#include "stm32f4xx.h"
#include <stdint.h>

void i2c1_init(void);
void i2c1_read(char slaveAddress, char memoryAddress, char* buffer, int numberOfBytes);
void i2c1_write(char slaveAddress, char memoryAddress, char* data, int numberOfBytes);

#endif