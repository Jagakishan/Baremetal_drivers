#include "gpTimer.h"

void tim2_init(void){
    RCC->APB1ENR |= (1U<<0);

    TIM2->PSC = 16000-1;
    TIM2->ARR = 10000-1;

    TIM2->CNT=0;

    TIM2->CR1 |= (1U<<0);
}

bool tim2_done(void){
    return (TIM2->SR & (1U<<0));
}