#include "rtc.h"

#define ASYNCH_PRE_VAL 127 //128-1
#define SYNCH_PRE_VAL 255 //256-1

static void enter_init_mode(void){
    RTC->ISR |= (1U<<7); //Init mode

    while(!(RTC->ISR & (1U<<6))){} //Wait for init mode confirmation

    return;
}

static void exit_init_mode(void){
    RTC->ISR &= ~(1U<<7); // Exit Init mode

    while((RTC->ISR & (1U<<6))){} //Wait for free run mode confirmation

    return;
}

static void set_asynch_prescale_value(void){
    RTC->PRER &= ~(127<<16);
    RTC->PRER |= (ASYNCH_PRE_VAL<<16);
}

static void set_synch_prescale_value(void){
    RTC->PRER &= ~(32767<<0);
    RTC->PRER |= (SYNCH_PRE_VAL<<0);
}

static void set_date(uint32_t weekDay, uint32_t day, uint32_t month, uint32_t year){
    uint32_t temp=0;

    temp=((day & 0x0F)<<0) | ((day & 0xF0)<<4)
        | ((month & 0x0F)<<8) | ((month & 0xF0)<<12)
        | ((weekDay)<<13)
        | ((year & 0x0F)<<16) | ((year & 0xF0)<<20);

    RTC->DR = temp;

    return;
}

static void set_time(uint32_t seconds, uint32_t minutes, uint32_t hours, uint32_t format){
    uint32_t temp=0;

    temp = ((seconds & 0x0F)<<0) | ((seconds & 0xF0)<<4)
        | ((minutes & 0x0F)<<8) | ((minutes & 0xF0)<<12)
        | ((hours & 0x0F)<<16) | ((hours & 0xF0)<<20)
        | (format<<22);

    RTC->TR=temp;

    return;
}

static uint8_t bcd2dec(uint8_t bcdVal){
    return (uint8_t)(((bcdVal&0xF0)>>4)*10U + (bcdVal&0x0F));
}

void rtc_init(uint32_t weekDay, uint32_t day, uint32_t month, uint32_t year, 
                uint32_t seconds, uint32_t minutes, uint32_t hours, uint32_t format){

    RCC->APB1ENR |= (1U<<28); //PWR peripheral enable

    PWR->CR |= (1U<<8); //Disable backup domain write protection to config. RTC

    RCC->BDCR |= (1U<<16);
    RCC->BDCR &= ~(1U<<16); //Reset the backup domain and again start for proper working.

    RCC->BDCR |= (1U<<0); //LSE on
    while(!(RCC->BDCR & (1U<<1))){} //wait for LSE to be ready 

    RCC->BDCR |= (1U<<8);
    RCC->BDCR &= ~(1U<<9); //RTC clock sel->LSE

    RCC->BDCR |= (1U<<15); //Enable RTC

    RTC->WPR = 0xCAU;
    RTC->WPR = 0x53U; //Disable RTC write protection

    enter_init_mode();

    set_asynch_prescale_value();

    set_synch_prescale_value();

    set_date(weekDay, day, month, year);

    set_time(seconds, minutes, hours, format);

    exit_init_mode();

    RTC->WPR = 0xFFU; //enable RTC write protection
}

uint8_t get_year(void){
    uint8_t temp;
    temp=(uint8_t)((RTC->DR & 0x00FF0000)>>16);

    return bcd2dec(temp);
}

uint8_t get_month(void){
    uint8_t temp;
    temp=(uint8_t)((RTC->DR & 0x00001F00)>>8);

    return bcd2dec(temp);
}

uint8_t get_day(void){
    uint8_t temp;
    temp=(uint8_t)((RTC->DR & 0x0000003F)>>0);

    return bcd2dec(temp);
}

uint8_t get_weekDay(void){
    uint8_t temp;
    temp=(uint8_t)((RTC->DR & 0x0000E000)>>13);

    return bcd2dec(temp);
}

uint8_t get_hours(void){
    uint8_t temp;
    temp=(uint8_t)((RTC->TR & 0x003F0000)>>16);

    return bcd2dec(temp);
}

uint8_t get_minutes(void){
    uint8_t temp;
    temp=(uint8_t)((RTC->TR & 0x00007F00)>>8);

    return bcd2dec(temp);
}

uint8_t get_seconds(void){
    uint8_t temp;
    temp=(uint8_t)((RTC->TR & 0x0000007F)>>0);

    return bcd2dec(temp);
}