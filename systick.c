#include "systick.h"

void delay(uint32_t delay){
    SysTick->CTRL |= (1U<<2); //Choose internal processor clock for sysTick

    SysTick->LOAD = 16000-1; //1ms delay, load value 16000 cycles for 16MHz clock speed

    SysTick->VAL = 0; //Writing to VAL clears the count in sysTick

    SysTick->CTRL |= (1U<<0); //Start tick

    for(uint32_t i=0;i<delay;i++){
        while(!(SysTick->CTRL & (1U<<16))){}; //Check for reload
    }

    SysTick->CTRL &= ~(1U<<0); //Disable system tick timer

}