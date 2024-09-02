#ifndef _MAIN_H_
#define _MAIN_H_

#include <stm32f0xx.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "gpio.h"
#include "i2c.h"
#include "ssd1306.h"

void timers0_init(void);
void timers90_init(void);
void upd_init(void);
void delay (volatile uint32_t);
uint16_t adc_read( void );
void adc_init( void );
void clock_init(void);

#endif
