#include "i2c.h"
#include "gpio.h"

/*
* П Р О Г Р А М М Н А Я   Р Е А Л И З А Ц И Я   I 2 C 1
* I2C SCL PA4 (10)
* I2C SDA PA3 (9)
*/

void I2C_Init(void)
{
#define TD1 1
#define rlsSCL1 GPIOA->MODER &= ~GPIO_MODER_MODER4; twi_delay(TD1);
#define pdnSCL1 GPIOA->MODER &= ~GPIO_MODER_MODER4; GPIOA->MODER |= GPIO_MODER_MODER4_0; GPIOA->OTYPER |= GPIO_OTYPER_OT_4; GPIOA->BRR |= GPIO_BRR_BR_4; twi_delay(TD1);
#define rlsSDA1 GPIOA->MODER &= ~GPIO_MODER_MODER3; twi_delay(TD1);
#define pdnSDA1 GPIOA->MODER &= ~GPIO_MODER_MODER3; GPIOA->MODER |= GPIO_MODER_MODER3_0; GPIOA->OTYPER |= GPIO_OTYPER_OT_3; GPIOA->BRR |= GPIO_BRR_BR_3; twi_delay(TD1);
RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // clock port A
rlsSDA1;rlsSCL1;
}

void twi_delay( volatile uint32_t dly ){
  for( volatile uint32_t i = 0; i < dly; i++ ){}
  }

void twi_start(void)
{
rlsSDA1;rlsSCL1;
pdnSDA1;pdnSCL1;
}

void twi_stop(void)
{
rlsSCL1;rlsSDA1;
}

void twi_write( char twi_byte)
{
for( uint8_t i = 0; i < 8; i++ )
  {
  if(twi_byte&0b10000000) {rlsSDA1;}
  else { pdnSDA1;}
  rlsSCL1;  pdnSCL1;
  twi_byte <<= 1;
  }
rlsSDA1;rlsSCL1; // skip ack
pdnSCL1;pdnSDA1; // mid state
}

void I2C_Send(char bits[], char Address)
{
twi_start();
twi_write(Address<<1);
twi_write(bits[0]);
twi_write(bits[1]);
twi_stop();
}
