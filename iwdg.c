#include "iwdg.h"

#define WDG_START_KEY 0x0000CCCC
#define WDG_KICKTHEDOG_KEY 0x0000AAAA
#define WDG_WP_DISABLE_KEY 0x00005555

void iwdg_init(void){
    IWDG->KR = WDG_START_KEY; //Enable the dog

    IWDG->KR = WDG_WP_DISABLE_KEY; //Disable write protection

    IWDG->PR =0; //LSI clk/4 by writing 000 at last 3 bits. However reset value is all 0's.

    IWDG->RLR = 0xFFF; //Reload value for the down counter after every reset

    while((IWDG->SR & (1U<<0)) || (IWDG->SR & (1U<<1))){} //Wait till RLR and PRER are set

    IWDG->KR = WDG_KICKTHEDOG_KEY; 
}