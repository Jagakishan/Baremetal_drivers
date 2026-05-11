#include "adc.h"

void pa1_adc_init(void){
    RCC->AHB1ENR |= (1U<<0); //GPIOA

    GPIOA->MODER |= (1U<<2);
    GPIOA->MODER |= (1U<<3); //Analog mode

    RCC->APB2ENR |= (1U<<8); //ADC1 

    ADC1->CR1 &= ~(1U<<25);
    ADC1->CR1 &= ~(1U<<24); //12 bit ADC

    ADC1->CR2 |= (1U<<1); //Continuous conversion mode

    ADC1->SQR3 |= (1U<<0); //0001 to specify channel 1 as first conversion

    ADC1->SQR1 = 0x0; //Only 1 conversion. Sequence length set to 0. (L=0 means 1)

    ADC1->SMPR2 &= ~(7<<3);
    ADC1->SMPR2 |= (4<<3); //Sampling time 84 cycles

    ADC1->CR2 |= (1U<<0); //Turn ADC on

}

void pa1_start_sampling(void){
    ADC1->CR2 |= (1U<<30); //Start conversion
}

uint32_t pa1_adc_read(void){
    while(!(ADC1->SR & (1U<<1))){}

    return (ADC1->DR);
}