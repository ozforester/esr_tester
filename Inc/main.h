// Copyright ozforester, MIT License

#ifndef _MAIN_H_
#define _MAIN_H_

#define PHASE 0
#define FILLER "       "
#define VCC 3.0f /* опорное ацп */
#define PWM 3.0f /* напряжение шима */
#define ADB 4095.0f
#define NOICE 210 /* ESR выборки при шунте на измерительном входе */
#define OPA 84.0f /* 104 реальных при расчетном 84, влияние интегратора и щупов осциллографа*/
#define LPF 1.64f /* ESR mode lpf and switch loss */
#define CAP 0.13f /* ESR pwm serial capacitor impedance 120k */
#define R1 150.0f + CAP  /* resistor and capacitor in serial */
#define R2 7.5f
#define R23 0.0 /* ESR остаток на тестовом порту при шунте */
#define PCB 0.00f /* loss on pcb and test port */

#include <stm32f0xx.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "gpio.h"
#include "i2c.h"
#include "ssd1306.h"

void timers_init(void);
void upd_init(void);
void delay (volatile uint32_t);
uint16_t adc_read( void );
void adc_init( void );
void clock_init(void);

#endif
