// Copyright ozforester, MIT License

#include "main.h"

char s[32] ;
volatile int adc;
volatile float uadc ;
volatile int frac ;
volatile float u23 ;
volatile float r23 ;
volatile float r3 ;
volatile float esr ;
volatile float cap ;

int main()
{
	clock_init();
	adc_init();
	GPIO_Init();
	I2C_Init();
	SSD1306_Init();
        timers_init();
	while(1)
		{
		delay(100);
                for( volatile uint8_t i = 0 ; i < 100 ; i++ ){ adc += adc_read() ; } adc /= 100 ; // multi
		adc -= NOICE ;
		adc = (adc<0) ? 0 : adc ; // проверка знака
		uadc = adc * VCC / ADB ;
		u23 = uadc / OPA * LPF  ;
		r23 = u23 * R1 / ( PWM - u23 ) - R23 ;
		r3 = r23 * R2 / (R2 - r23) ;
		//r3 = (r3 < 0) ? 0 : r3 ;// open port
		esr = r3 - PCB ;
                if(esr >= 0 &&  esr < 100 )
		{
                frac = (esr-(int)esr)*100 ;
                itoa( (int)esr, s, 10);
                ssd1306_SetCursor(0, 0);
                ssd1306_print(FILLER, Font_16x26, White);
                ssd1306_SetCursor(0, 0);
                ssd1306_print(s, Font_16x26, White);
                ssd1306_print(".", Font_16x26, White);
                if(frac<10) ssd1306_print("0", Font_16x26, White);
                itoa( frac, s, 10);
                ssd1306_print(s, Font_16x26, White);
                ssd1306_UpdateScreen();
		}
		else
		{
                ssd1306_SetCursor(0, 0);
                ssd1306_print(FILLER, Font_16x26, White);
                ssd1306_SetCursor(0, 0);
                ssd1306_print("-- --", Font_16x26, White);
                ssd1306_UpdateScreen();
		}
 	}
}

// EOF
