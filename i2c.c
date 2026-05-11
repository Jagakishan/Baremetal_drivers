#include "i2c.h"

#define I2C_CLK_CCR 80
#define CLK_RISE_TIME 17

void i2c1_init(void){
    RCC->AHB1ENR |= (1U<<1); //GPIOB

    GPIOB->MODER |= (1U<<13);
    GPIOB->MODER &= ~(1U<<12); //PB6 as AF

    GPIOB->MODER |= (1U<<15);
    GPIOB->MODER &= ~(1U<<14); //PB7 as AF

    GPIOB->AFR[0] &= ~(15<<24);
    GPIOB->AFR[0] |= (4<<24); //AF as I2C1 Clk for PB6

    GPIOB->AFR[0] &= ~(15<<28);
    GPIOB->AFR[0] |= (4<<28); //AF as I2C1 data for PB7

    GPIOB->OTYPER |= (1U<<6);
    GPIOB->OTYPER |= (1U<<7); //Open drain configuration

    GPIOB->PUPDR &= ~(1U<<13);
    GPIOB->PUPDR |= (1U<<12); 

    GPIOB->PUPDR &= ~(1U<<15);
    GPIOB->PUPDR |= (1U<<14); //Pull up for SCL and SDA

    RCC->APB1ENR |= (1U<<21); //i2c1

    I2C1->CR1 |= (1U<<15); //Enter reset mode
    I2C1->CR1 &= ~(1U<<15); //Exit reset mode. To freshen up the I2C peripheral 

    I2C1->CR2 &= ~(31<<0);
    I2C1->CR2 |= (16<<0); //16MHz peripheral clock

    I2C1->CCR = I2C_CLK_CCR;

    I2C1->TRISE = CLK_RISE_TIME;

    I2C1->CR1 |= (1U<<0); //I2C enable
}

void i2c1_read(char slaveAddress, char memoryAddress, char* buffer, int numberOfBytes){
    uint32_t temp;

    while(I2C1->SR2 & (1U<<1)){} //Wait till busy flag resets

    I2C1->CR1 |= (1U<<8); //Start
    while(!(I2C1->SR1 & (1U<<0))){} //Ack for start

    I2C1->DR = slaveAddress<<1 | 0; //Send slave address, with write mode
    while(!(I2C1->SR1 & (1U<<1))){} //Wait for Ack for address

    temp=I2C1->SR2; //Clear address flag

    while(!(I2C1->SR1 &(1<<7))){} //Wait till Tx Buffer empty

    I2C1->DR = memoryAddress; //Send memory address to be read from
    while(!(I2C1->SR1 &(1<<7))){} // Wait till Tx Buffer empty

    I2C1->CR1 |= (1U<<8); //Restart I2C cycle
    while(!(I2C1->SR1 & (1U<<0))){} // wait for start ack

    I2C1->DR = slaveAddress<<1 | 1; //Send slave address, with read mode
    while(!(I2C1->SR1 & (1U<<1))){} //Wait for Ack for address

    temp=I2C1->SR2; //Clear address flag

    I2C1->CR1 |= (1U<<10); //Enable data ACKs

    for(int i=0;i<numberOfBytes;i++){
        if(i==numberOfBytes-1){
            I2C1->CR1 &= ~(1U<<10); //Disable Ack. No Ack for last byte

            I2C1->CR1 |= (1U<<9); // stop condition

            while(!(I2C1->SR1 & (1U<<6))){} //Wait for RxNE
            buffer[i]=I2C1->DR; //Read Rx buffer(same buffer used for Tx)
        }

        while(!(I2C1->SR1 & (1U<<6))){} //Wait for RxNE
        buffer[i]=I2C1->DR; //Read Rx buffer(same buffer used for Tx)
    }
}

void i2c1_write(char slaveAddress, char memoryAddress, char* data, int numberOfBytes){
    uint32_t temp;

    while(I2C1->SR2 & (1U<<1)){} //Wait till busy flag resets

    I2C1->CR1 |= (1U<<8); //Start
    while(!(I2C1->SR1 & (1U<<0))){} //Ack for start

    I2C1->DR = slaveAddress<<1 | 0; //Send slave address, with write mode
    while(!(I2C1->SR1 & (1U<<1))){} //Wait for Ack for address

    temp=I2C1->SR2; //Clear address flag

    while(!(I2C1->SR1 &(1<<7))){} //Wait till Tx Buffer empty

    I2C1->DR = memoryAddress; //Send memory address to write to
    
    for(int i=0;i<numberOfBytes;i++){
        while(!(I2C1->SR1 &(1<<7))){} //Wait till Tx buf empty

        I2C1->DR = data[i]; //Send next byte
    }

    while(!(I2C1->SR1 & (1U<<2))){} //Wait for Byte transfer finish signal

    I2C1->CR1 |= (1U<<9); // stop condition
}