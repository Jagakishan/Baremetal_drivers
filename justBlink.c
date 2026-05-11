//Defining addresses
#define PERIPHERAL_BASE (0x40000000UL)
#define AHB1_BASE (0x40020000UL)
#define GPIOA_BASE (0x40020000UL)
#define RCC_BASE (0x40023800UL)
#define AHB1_ENR_OFFSET (0x30UL)
#define AHB1_ENR_BASE (RCC_BASE+AHB1_ENR_OFFSET)
#define GPIOA_MODER_OFFSET (0x0UL)
#define GPIOA_MODER_BASE (GPIOA_BASE+GPIOA_MODER_OFFSET)
#define GPIOA_ODR_OFFSET (0x14UL)
#define GPIOA_ODR_BASE (GPIOA_BASE+GPIOA_ODR_OFFSET)

//Setting up registers whose value we might need
#define AHB1_ENR (*(volatile unsigned int *)AHB1_ENR_BASE)
#define GPIOA_MODER (*(volatile unsigned int *)GPIOA_MODER_BASE)
#define GPIOA_ODR (*(volatile unsigned int *)GPIOA_ODR_BASE)

int main(){
    //Enable clock for GPIOA alone
    AHB1_ENR |= (1<<0);

    //Set output mode for LED Pin(PA5)
    //Bit 11 and 10 should be 01 for output mode of Pin 5 of GPIOA
    GPIOA_MODER &= ~(1<<11);
    GPIOA_MODER |= (1<<10);

    while(1){
        GPIOA_ODR ^= (1<<5);
        for(int i = 0; i < 100000; i++){}; //Some amount of delay 
    }

}
