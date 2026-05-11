#ifndef RTC_H
#define RTC_H

#include "stm32f4xx.h"
#include <stdint.h>

void rtc_init(uint32_t weekDay, uint32_t day, uint32_t month, uint32_t year, 
                uint32_t seconds, uint32_t minutes, uint32_t hours, uint32_t format);

uint8_t get_year(void);
uint8_t get_month(void);
uint8_t get_day(void);
uint8_t get_weekDay(void);
uint8_t get_hours(void);
uint8_t get_minutes(void);
uint8_t get_seconds(void);

#endif