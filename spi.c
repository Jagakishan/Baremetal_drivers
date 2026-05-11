#include "spi.h"

void spi_init(void){
    RCC->AHB1ENR |= (1U<<0); //GPIOA

    GPIOA->MODER |= (1U<<11);
    GPIOA->MODER &= ~(1U<<10); //PA5 as AF

    GPIOA->MODER |= (1U<<13);
    GPIOA->MODER &= ~(1U<<12); //PA6 as AF

    GPIOA->MODER |= (1U<<15);
    GPIOA->MODER &= ~(1U<<14); //PA7 as AF

    GPIOA->MODER |= (1U<<8);
    GPIOA->MODER &= ~(1U<<9); //PA4 as output for Chip Select

    GPIOA->AFR[0] &= ~(15<<20);
    GPIOA->AFR[0] |= (5<<20); //PA5 as SPI1 as AF (AF5)

    GPIOA->AFR[0] &= ~(15<<24);
    GPIOA->AFR[0] |= (5<<24); //PA6 as SPI1 as AF (AF5)

    GPIOA->AFR[0] &= ~(15<<28);
    GPIOA->AFR[0] |= (5<<28); //PA7 as SPI1 as AF (AF5)

    RCC->APB2ENR |= (1U<<12); //SPI1

    SPI1->CR1 |= (1U<<0);
    SPI1->CR1 |= (1U<<1); //CPHA and CPOL 1. Clk idle high and sample at rising edge

    SPI1->CR1 &= ~(7<<3);
    SPI1->CR1 |= (1<<3); //Bit rate to fclk/4

    SPI1->CR1 &= ~(1U<<7); //MSB first

    SPI1->CR1 &= ~(1U<<11); //8 bit data format

    SPI1->CR1 |= (1U<<13); //CRC enabled

    SPI1->CR1 |= (1U<<8);
    SPI1->CR1 |= (1U<<9); //Software slave management

    SPI1->CR1 |= (1U<<2); //Set to Master

    SPI1->CR1 &= ~(1U<<10); //Full Duplex

    SPI1->CR1 |= (1U<<6); //Enable SPI1
}

void spi1_transmit(uint8_t* data, int size){
    for(int i=0;i<size;i++){
        while(!(SPI1->SR & (1U<<1))){}

        SPI1->DR=data[i];
    }

    while(!(SPI1->SR & (1U<<1))){} //Wait for last byte transmission.

    while(!(SPI1->SR & (1U<<7))){} //Wait for busy flag to be cleared 

    uint32_t temp;

    temp=SPI1->DR;
    temp=SPI1->SR; //Clears DR and SR for further data reception or transmission.  
}

void spi1_receive(uint8_t* data, int size){
    while(size){
        SPI1->DR=0; //Send dummy byte to start generating clock pulses

        while(!(SPI1->SR & (1U<<0))){}

        *data++=SPI1->DR;
        size--;
    }
}

void pa4_slave_enable(void){
    GPIOA->ODR &= ~(1U<<4); //Pull low
}

void pa4_slave_disable(void){
    GPIOA->ODR |= (1U<<4); //Pull high
}