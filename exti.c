#include "exti.h"

void pc9_exti_init(void){
    __disable_irq(); //Disable all interrupts globally

    RCC->AHB1ENR |= (1U<<2); //GPIOC

    GPIOC->MODER &= ~(1U<<18);
    GPIOC->MODER &= ~(1U<<19); //Input
 
    RCC->APB2ENR |= (1U<<14); //SYSCFG

    SYSCFG->EXTICR[2] &= ~(15<<3);
    SYSCFG->EXTICR[2] |= (2<<3); //Map EXTI9 to PC9

    EXTI->IMR |= (1U<<9); //Unmask EXTI9

    EXTI->RTSR |= (1U<<9); //Rising edge enabled

    NVIC_EnableIRQ(EXTI9_5_IRQn); //Enable NVIC line group corresponding to EXTI9

    __enable_irq(); //Enable all interrupts
}