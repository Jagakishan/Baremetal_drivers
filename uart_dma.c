#include "uart_dma.h"

#define SYSTEM_CLOCK 16000000
#define BAUD_RATE 115200
#define RECEPTION_SIZE 10
#define TRANSMISSION_SIZE 20

uint32_t receive_buffer[RECEPTION_SIZE];
uint32_t transmit_buffer[TRANSMISSION_SIZE];

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

    USART2->CR3 |= (1U<<7); //DMA for Transmission
    USART2->CR1 |= (1U<<6); //DMA for reception

    //Transmitter and receiver enabled
    USART2->CR1 |= (1U<<3);
    USART2->CR1 |= (1U<<2);

    USART2->SR &= ~(1U<<6); //Clear TC flag

    USART2->CR1 |= (1U<<6); //TC Interrupt enabled

    //USART2 enabled
    USART2->CR1 |= (1U<<13);

    NVIC_EnableIRQ(USART2_IRQn); //NVIC line for UART2 is enabled
}

void dma_uart_init(void){
    RCC->AHB1ENR |= (1U<<21); //DMA1 clock enable

    DMA1->HIFCR =0b00000000001111010000111101000000; //Clear interrupt flags at start

    //UART2 RX is DMA1 STREAM5
    DMA1_Stream5->CR &= ~(1U<<0); //Disable stream 5 for config
    while(DMA1_Stream5->CR & (1U<<0)){} 

    DMA1_Stream5->CR |= (1U<<4); //TCIE

    DMA1_Stream5->CR |= (1U<<8); //Circ mode
    DMA1_Stream5->CR |= (1U<<10); //Mem Inc mode

    DMA1_Stream5->CR |= (1U<<14);
    DMA1_Stream5->CR &= ~(1U<<13); //32 bit word size for transfer into memory

    DMA1_Stream5->CR |= (1U<<12);
    DMA1_Stream5->CR &= ~(1U<<11); //32 bit word size for transfer from peripheral

    DMA1_Stream5->CR |= (1U<<27); //Channel 4 select

    DMA1_Stream5->NDTR = (uint16_t)RECEPTION_SIZE;

    DMA1_Stream5->M0AR = ((uint32_t)(receive_buffer)); //Mem address
    DMA1_Stream5->PAR = (uint32_t)(&USART2->DR); //peripheral address

    DMA1_Stream5->CR |= (1U<<0); //Enable stream 5

    NVIC_EnableIRQ(DMA1_Stream5_IRQn);

    //UART2 TX is DMA1 STREAM6
    DMA1_Stream6->CR &= ~(1U<<0); //Disable stream 6 for config
    while(DMA1_Stream6->CR & (1U<<0)){} 

    DMA1_Stream6->CR |= (1U<<4); //TCIE

    DMA1_Stream6->CR |= (1U<<8); //Circ mode
    DMA1_Stream6->CR |= (1U<<10); //Mem Inc mode

    DMA1_Stream6->CR |= (1U<<14);
    DMA1_Stream6->CR &= ~(1U<<13); //32 bit word size for transfer into memory

    DMA1_Stream6->CR |= (1U<<12);
    DMA1_Stream6->CR &= ~(1U<<11); //32 bit word size for transfer from peripheral

    DMA1_Stream6->CR |= (1U<<27); //Channel 4 select

    DMA1_Stream6->NDTR = (uint16_t)TRANSMISSION_SIZE;

    DMA1_Stream6->CR |= (1U<<6); //Mem to peripheral direction

    DMA1_Stream6->M0AR = ((uint32_t)(transmit_buffer)); //Mem address
    DMA1_Stream6->PAR = (uint32_t)(&USART2->DR); //peripheral address

    DMA1_Stream6->CR |= (1U<<0); //Enable stream 5

    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}