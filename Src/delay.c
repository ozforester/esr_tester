#include <stdint.h>
void delay(volatile uint32_t ms)
{
	while(ms--);
}
