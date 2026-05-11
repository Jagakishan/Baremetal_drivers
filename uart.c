#include "uart.h"

#define SYSTEM_CLOCK 16000000
#define BAUD_RATE 115200

static void uart_transmit(int ch){
    while(!(USART2->SR & (1U<<7))){}

    USART2->DR = ch && 0xFF;
}

int __io_putchar(int ch){
    uart_transmit(ch);

    return ch;
}

void uart_read(uint8_t* buffer, int numberOfBytes){
    for(int i=0;i<numberOfBytes;i++){
        while(!(USART2->SR & (1U<<5))){}

        if((USART2->SR & (1U<<3)) || (USART2->SR & (1U<<0))){
            continue;
        }

        buffer[i] = (USART2->DR & 0xFF);
    }

    return;
}

static uint32_t computeBaudRate(uint32_t baudrate){
    return ((SYSTEM_CLOCK+BAUD_RATE/2U)/BAUD_RATE);
}

static void setBaudRate(uint32_t baudrate){
    USART2->BRR = computeBaudRate(baudrate);
}

void uart_init(void){
    RCC->AHB1ENR |= (1U<<0); //GPIOA clock enabled

    //PA2 as UART2 TX
    GPIOA->MODER |= (1u<<5);
    GPIOA->MODER &= ~(1U<<4);

    GPIOA->AFR[0] |= (1U<<8);
    GPIOA->AFR[0] |= (1U<<9);
    GPIOA->AFR[0] |= (1U<<10);
    GPIOA->AFR[0] &= ~(1U<<11);

    //PA3 as UART2 RX
    GPIOA->MODER |= (1u<<7);
    GPIOA->MODER &= ~(1U<<6);

    GPIOA->AFR[0] |= (1U<<12);
    GPIOA->AFR[0] |= (1U<<13);
    GPIOA->AFR[0] |= (1U<<14);
    GPIOA->AFR[0] &= ~(1U<<15);

    RCC->APB1ENR |= (1U<<17); //UART2

    setBaudRate(BAUD_RATE);

    //Transmitter and receiver enabled
    USART2->CR1 |= (1U<<3);
    USART2->CR1 |= (1U<<2);

    //USART2 enabled
    USART2->CR1 |= (1U<<13);
}