#include "stm32f0xx.h"
void clock_init(void)
{
        FLASH->ACR |= FLASH_ACR_PRFTBE; // enables prefetch buffer
        FLASH->ACR |= FLASH_ACR_LATENCY ; // set latency to 24 - 48 MHz
        RCC->CR &= ~RCC_CR_PLLON; // PLL off
        while( RCC->CR & RCC_CR_PLLRDY ); // wait PLL stop
        RCC->CFGR |= (0b1010 << 18); // PLL = 4 or 8/2 x 12 = 48 MHz
        RCC->CR |= RCC_CR_PLLON; // PLL on
        while( !(RCC->CR & RCC_CR_PLLRDY ) ); // wait PLL ready
        RCC->CFGR |= RCC_CFGR_SW_1; // set PLL as system clock
        while ( !((RCC->CFGR) & RCC_CFGR_SWS_1) ); // wait till clock switched to PLL
}
