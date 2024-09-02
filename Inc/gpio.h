#ifndef GPIO_H
#define GPIO_H

#include "stm32f0xx.h"
#include "stm32f030x6.h"

//Defines
#define General_Purpose_Output 0b01

//functions
void GPIO_Init(void);
void set_Bit(GPIO_TypeDef* GPIOx,int Pin);
void reset_Bit(GPIO_TypeDef* GPIOx,int Pin);

#endif

