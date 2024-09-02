/*
Берите люди, пользуйтесь,
если без претензий.
*
*/
#include "main.h"
char s[20];
volatile uint8_t update = 0 ;
void TIM17_IRQHandler(void)
{update=1;TIM17->SR=0;}
int main()
{
	clock_init();
	adc_init();
        upd_init();
	GPIO_Init();
	I2C_Init();
	SSD1306_Init();
        uint16_t adcmin =  99;
        timers0_init();
	while(1)
	{
		if( update == 1)
		{
                volatile int adc;
                adc = adc_read() ;
		adc -= adcmin ;
		adc = (adc<0) ? 0 : adc ;
		#define VCC 3.0
		#define DIV 3.0
		#define QL 4096
		#define OPA 84
		#define R1 994
		#define R2 47.5
		#define L 4
		#define PCB 0.25
		volatile float u23 = (VCC * adc * L) / (QL * OPA ) ;
		volatile float i = (DIV - u23) / R1 ;
		volatile float esr = u23 / (i - u23 / R2) ;
		esr -= 0.25 ;
                esr = (esr<0) ? 0 : esr ;
                volatile int frac = (esr-(int)esr)*100 ;
                esr = (esr>99) ? 99 : esr ;
                itoa( (int)esr, s, 10);
                ssd1306_SetCursor(26, 0);
                if(esr<10) ssd1306_print(" ", Font_16x26, White);
                ssd1306_print(s, Font_16x26, White);
                ssd1306_print(".", Font_16x26, White);
                frac = (esr>=99) ? 99 : frac ;
		if(frac<10) ssd1306_print("0", Font_16x26, White);
                itoa( frac, s, 10);
                ssd1306_print(s, Font_16x26, White);
                if(frac<10) ssd1306_print(" ", Font_16x26, White);
		ssd1306_UpdateScreen();
		update = 0 ;
		}
	}
}
