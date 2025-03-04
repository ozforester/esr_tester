// Copyright ozforester, MIT License

#include <stdint.h>
void delay(volatile uint32_t ms)
{
	volatile uint32_t i = ms * 5000 ;
	while(i--);
}
