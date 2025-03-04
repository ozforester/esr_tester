// Copyright ozforester, MIT License

#include "main.h"

void timers_init(void) // Master
{
        // PWM - TIM1 CH3 PA10 (18) AF2
        RCC->AHBENR  |= RCC_AHBENR_GPIOAEN; // clock port A
        GPIOA->MODER  &= ~GPIO_MODER_MODER10; // reset PA10
        GPIOA->MODER  |= GPIO_MODER_MODER10_1; // PA10 AF
        GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR10_1|GPIO_OSPEEDR_OSPEEDR10_0; // PA10 HIGH SPEED
        GPIOA->AFR[1]  |= 0b0010 << GPIO_AFRH_AFSEL10_Pos; // PA10 AF2
        RCC->APB2ENR   |= RCC_APB2ENR_TIM1EN; // clocking TIM
        TIM1->CR2	|= TIM_CR2_MMS_0 ; // enable trigger
#if (PHASE==90)
        TIM1->CNT   = 10000; // slave delay correction + 1/2 pi
        TIM1->ARR   = 40000 ; // 1200 Hz
#else
        TIM1->CNT   = 8 ; // slave delay tune
        TIM1->ARR   = 400 ; // 1200 Hz
#endif
        TIM1->CCR3  = TIM1->ARR / 2 ; // duty ARR/2
        TIM1->CCMR2  |= TIM_CCMR2_OC3M_2|TIM_CCMR2_OC3M_1|TIM_CCMR2_OC3M_0 ; // pwm mode 2
        TIM1->CCER    |= TIM_CCER_CC3E ;    // CC3 output enable
        TIM1->BDTR |= TIM_BDTR_MOE ; // moe-bit
        // CTRL - TIM3 CH2 PA7 (13) AF1
        RCC->AHBENR  |= RCC_AHBENR_GPIOAEN; // clock port A
        GPIOA->MODER  &= ~GPIO_MODER_MODER7; // reset PA7
        GPIOA->MODER  |= GPIO_MODER_MODER7_1; // PA7 AF
        GPIOA->AFR[0]  |= 0b0001 << GPIO_AFRL_AFSEL7_Pos; // PA7 AF1
        RCC->APB1ENR   |= RCC_APB1ENR_TIM3EN; // clocking TIM
        TIM3->SMCR	|= TIM_SMCR_SMS_2|TIM_SMCR_SMS_1 ; // start w/o reset
        TIM3->SMCR	|= TIM_SMCR_SMS_2|TIM_SMCR_SMS_1 ; // start w/o reset
        //TIM3->CNT = 10000 ; // pi/2
        TIM3->ARR   = TIM1->ARR; //
        TIM3->CCR2  = TIM1->ARR / 2 ; // duty ARR/2
        TIM3->CCMR1  |= TIM_CCMR1_OC2M_2|TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2M_0 ; // pwm mode 2
        TIM3->CCER    |= TIM_CCER_CC2E ;    // CC2 output enable
        TIM3->BDTR |= TIM_BDTR_MOE ; // moe-bit
        TIM3->CR1    |= TIM_CR1_CEN ; // enable timer
        TIM1->CR1    |= TIM_CR1_CEN ; // enable timer (master)
}
