#include "gpio.h"

//Output driver
void led_init(void){
    RCC->AHB1ENR |= (1U<<0);

    GPIOA->MODER |= (1U<<10);
    GPIOA->MODER &= ~(1U<<11);
}

void led_on(void){
    GPIOA->BSRR |= (1U<<5);
}

void led_off(void){
    GPIOA->BSRR |= (1U<<21);
}

//Input driver. User button on dev board
void button_init(void){
    RCC->AHB1ENR |= (1U<<2);

    GPIOC->MODER &= ~(1U<<26);
    GPIOC->MODER &= ~(1U<<27);
}

bool button_state(void){
    if(GPIOC->IDR & (1<<13)){
        return false;
    }else{
        return true;
    }
}



