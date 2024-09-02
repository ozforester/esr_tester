#include "stm32f0xx.h"

void timers0_init(void) // Master
{
        RCC->AHBENR  |= RCC_AHBENR_GPIOAEN; // clock port A
        GPIOA->MODER  &= ~GPIO_MODER_MODER10; // reset PA10
        GPIOA->MODER  |= GPIO_MODER_MODER10_1; // PA10 AF
        GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR10_1|GPIO_OSPEEDR_OSPEEDR10_0; // PA10 HIGH SPEED
        GPIOA->AFR[1]  |= 0b0010 << GPIO_AFRH_AFSEL10_Pos; // PA10 AF2
        RCC->APB2ENR   |= RCC_APB2ENR_TIM1EN; // clocking TIM
	TIM1->CR2	|= TIM_CR2_MMS_0 ; // enable trigger
        TIM1->CNT = 13 ; // slave delay correction
        TIM1->ARR   = 400 - 1; // 120 kHz
        TIM1->CCR3  = 200 ; // duty ARR/2
        TIM1->CCMR2  |= TIM_CCMR2_OC3M_2|TIM_CCMR2_OC3M_1|TIM_CCMR2_OC3M_0 ; // pwm mode 2
        TIM1->CCER    |= TIM_CCER_CC3E ;    // CC3 output enable
        TIM1->BDTR |= TIM_BDTR_MOE ; // moe-bit
        RCC->AHBENR  |= RCC_AHBENR_GPIOAEN; // clock port A
        GPIOA->MODER  &= ~GPIO_MODER_MODER7; // reset PA7
        GPIOA->MODER  |= GPIO_MODER_MODER7_1; // PA7 AF
        GPIOA->AFR[0]  |= 0b0001 << GPIO_AFRL_AFSEL7_Pos; // PA7 AF1
        RCC->APB1ENR   |= RCC_APB1ENR_TIM3EN; // clocking TIM
        TIM3->SMCR	|= TIM_SMCR_SMS_2|TIM_SMCR_SMS_1 ; // start w/o reset
	TIM3->SMCR	|= TIM_SMCR_SMS_2|TIM_SMCR_SMS_1 ; // start w/o reset
        TIM3->ARR   = 400 - 1; //
        TIM3->CCR2  = 200 ; // duty ARR/2
        TIM3->CCMR1  |= TIM_CCMR1_OC2M_2|TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2M_0 ; // pwm mode 2
        TIM3->CCER    |= TIM_CCER_CC2E ;    // CC2 output enable
        TIM3->BDTR |= TIM_BDTR_MOE ; // moe-bit
        TIM3->CR1    |= TIM_CR1_CEN ; // enable timer
        TIM1->CR1    |= TIM_CR1_CEN ; // enable timer (master)
}

void upd_init(void)
{
        RCC->APB2ENR   |= RCC_APB2ENR_TIM17EN; // clocking TIM
        TIM17->PSC   = 48000 - 1; // 1000 Hz
        TIM17->ARR   = 1000 - 1; // 1 Hz
        TIM17->CCR1  = 500 ; // duty ARR/2
        TIM17->CR1    |= TIM_CR1_CEN ; // enable timer
        TIM17->DIER |= TIM_DIER_UIE ; // update isr
        NVIC_EnableIRQ( TIM17_IRQn );
}
