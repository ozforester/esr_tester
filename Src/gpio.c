#include "gpio.h"

void GPIO_Init()
{
	//Clocks------------------------------------------------
	//Enable_GPIOB_Clock
	RCC->AHBENR  |= RCC_AHBENR_GPIOBEN;
	//Set Output mode to General_Purpose_Output
	GPIOB->MODER |= (General_Purpose_Output)<<1 ;	
}

void set_Bit(GPIO_TypeDef* GPIOx,int Pin)
{
	GPIOx->BSRR |= 1 << Pin;
} 
void reset_Bit(GPIO_TypeDef* GPIOx,int Pin)
{
	GPIOx->BSRR |= 1 << (Pin+16);
}

