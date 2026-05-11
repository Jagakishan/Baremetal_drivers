#include "adc_dma.h"

#define NUMBER_OF_DATAITEMS 32

uint32_t adc_dma_receive_buffer[NUMBER_OF_DATAITEMS];

void adc_dma_init(void){
    RCC->AHB1ENR |= (1U<<0); //GPIOA

    GPIOA->MODER |= (1U<<2);
    GPIOA->MODER |= (1U<<3); //Analog mode PA1

    GPIOA->MODER |= (1U<<0);
    GPIOA->MODER |= (1U<<1); //Analog mode PA0

    RCC->APB2ENR |= (1U<<8); //ADC1 

    ADC1->CR1 &= ~(1U<<25);
    ADC1->CR1 &= ~(1U<<24); //12 bit ADC

    ADC1->CR1 |= (1U<<8); //Scan mode

    ADC1->SMPR2 &= ~(7<<3);
    ADC1->SMPR2 |= (4<<3); //Sampling time 84 cycles

    ADC1->SQR1 = 0x00100000; //ADC seq. length=2 conversions

    ADC1->SQR3 = 0x00000010; //channel 1 and 0 as 2nd and 1st conversion respectively.

    ADC1->CR2 |= (1U<<8); //DMA for ADC

    RCC->AHB1ENR |= (1U<<22); //DMA2 clock enable

    DMA2_Stream0->CR &= ~(1U<<0); //Disable stream for config.
    while(DMA2_Stream0->CR & (1U<<0)){} //Wait for disabling 

    DMA2_Stream0->CR |= (1U<<8); //Circular mode
    DMA2_Stream0->CR |= (1U<<10); //Memory address increment mode

    DMA2_Stream0->CR |= (1U<<14);
    DMA2_Stream0->CR &= ~(1U<<13); //32 bit word size for transfer into memory

    DMA2_Stream0->CR |= (1U<<12);
    DMA2_Stream0->CR &= ~(1U<<11); //32 bit word size for transfer from peripheral

    DMA2_Stream0->NDTR = (uint16_t)NUMBER_OF_DATAITEMS;

    DMA2_Stream0->M0AR = ((uint32_t)(adc_dma_receive_buffer)); //Mem address
    DMA2_Stream0->PAR = &((uint32_t)(ADC1->DR)); //peripheral address

    DMA2_Stream0->CR |= (1U<<0); //DMA2 stream0 enable

    ADC1->CR2 |= (1U<<0); //Turn ADC on

    ADC1->CR2 |= (1U<<30); //Start conversion
}