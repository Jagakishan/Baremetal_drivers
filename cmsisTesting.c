#include "stm32f4xx.h"

int main(){
    RCC->AHB1ENR=(1U<<0);

    GPIOA->MODER &= ~(1U<<11);
    GPIOA->MODER |= (1U<<10);

    while(1){
        GPIOA->ODR ^=(1U<<5);
        for(int i = 0; i < 800000; i++){};
    }
}