// Copyright ozforester, MIT License

#include "stm32f0xx.h"

//void delay(volatile uint32_t);
//char s[20];
//uint16_t adc = 0 ;
//uint32_t uV ;
//uint8_t update = 0 ;

uint16_t adc_read( void )
{
volatile int adc = 0 ;
for( volatile uint8_t i = 0 ; i < 250 ; i++ )
	{ // читаем 250 раз ацп
	while ((ADC1->ISR & ADC_ISR_ADRDY) == 0){}
	ADC1->CR |= ADC_CR_ADSTART;
       	while ((ADC1->ISR & ADC_ISR_EOC) == 0){}
	adc += ADC1->DR ;
	}
return( (uint16_t)(adc/250) ); // возвращаем среднее
}

void adc_init( void )
{ // ADC - ADC5 PA5 (11)
        RCC->AHBENR  |= RCC_AHBENR_GPIOAEN; // clock port A
        GPIOA->MODER  &= ~GPIO_MODER_MODER5; // reset PA5
        GPIOA->MODER  |= GPIO_MODER_MODER5_0|GPIO_MODER_MODER5_1; // PA5 Analog mode
        RCC->APB2ENR |= RCC_APB2ENR_ADCEN ;
        ADC1->CHSELR = ADC_CHSELR_CHSEL5 ; // channel 5
	//ADC1->CHSELR = ADC_CHSELR_CHSEL17 ; // Vref
	//ADC->CCR |= ADC_CCR_VREFEN ;
	ADC1->CFGR2 |= ADC_CFGR2_CKMODE_1 ; // PCLK/4 sync
	//ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2; // slow sampling
        ADC1->SMPR |= ADC_SMPR_SMP_0; // fast sampling
	if ((ADC1->CR & ADC_CR_ADEN) != 0) ADC1->CR |= ADC_CR_ADDIS;
	while ((ADC1->CR & ADC_CR_ADEN) != 0){}
	ADC1->CR |= ADC_CR_ADCAL; // calibration start
	while ((ADC1->CR & ADC_CR_ADCAL) != 0){} // calibration finish
	if ((ADC1->ISR & ADC_ISR_ADRDY) != 0) ADC1->ISR |= ADC_ISR_ADRDY; // enable
	ADC1->CR |= ADC_CR_ADEN; // enable
	while ((ADC1->ISR & ADC_ISR_ADRDY) == 0){} // wait till ready for request
}
