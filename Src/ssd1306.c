/* ssd1306 code origin: https://github.com/dhannyell/SSD1306-STM32F030F4-BareMetal
by Dhannyell Lukas. There were no any particular copyright notes
so I leave mine without notes too..
*/
#include "ssd1306.h"
#include <string.h>

typedef enum
{
	False = 0,
	True  = 1
} bool_t;

static char SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
//display object
SSD1306_t SSD1306;
static bool_t Inverted = 0;
void SSD1306_Init(void)
{
	I2C_Init();
	SSD1306_Send_Command(0xAE);
	SSD1306_Send_Command(0xAE); //display off
	SSD1306_Send_Command(0x20); //Set Memory Addressing Mode
	SSD1306_Send_Command(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	SSD1306_Send_Command(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	SSD1306_Send_Command(0xC0); //Set COM Output Scan Direction
	SSD1306_Send_Command(0x00); //---set low column address
	SSD1306_Send_Command(0x10); //---set high column address
	SSD1306_Send_Command(0x40); //--set start line address
	SSD1306_Send_Command(0x81); //--set contrast control register
	SSD1306_Send_Command(0x01);
	SSD1306_Send_Command(0xA0); //--set segment re-map 0 to 127
	SSD1306_Send_Command(0xA6); //--set normal display
	SSD1306_Send_Command(0xA8); //--set multiplex ratio(1 to 64)
	SSD1306_Send_Command(0x1F); // 31
	SSD1306_Send_Command(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	SSD1306_Send_Command(0xD3); //-set display offset
	SSD1306_Send_Command(0x00); //-not offset
	SSD1306_Send_Command(0xD5); //--set display clock divide ratio/oscillator frequency
	SSD1306_Send_Command(0xF0); //--set divide ratio
	SSD1306_Send_Command(0xD9); //--set pre-charge period
	SSD1306_Send_Command(0x22); //
	SSD1306_Send_Command(0xDA); //--set com pins hardware configuration
	SSD1306_Send_Command(0x02); // 32
	SSD1306_Send_Command(0xDB); //--set vcomh
	SSD1306_Send_Command(0x20); //0x20,0.77xVcc
	SSD1306_Send_Command(0x8D); //--set DC-DC enable
	SSD1306_Send_Command(0x14); //
	SSD1306_Send_Command(0xAF); //--turn on SSD1306 panel
	// Clear screen
	SSD1306_Fill(Black);

	// Flush buffer to screen
	ssd1306_UpdateScreen();

	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;

	SSD1306.Initialized = 1;
}


void SSD1306_Send_Command(char command)
{
	char b[2];
	b[0] = 0b00000000;
	b[1] = command;
	I2C_Send(b, SSD1306_Address);
}


void SSD1306_Send_Data(char data)
{
	char b[2];
	b[0] = 0b01000000;
	b[1]= data;
	I2C_Send(b, SSD1306_Address);
}


void SSD1306_Send_Array(char *Array, int size)
{
	for(volatile int i=0; i< size; i++)
	{
		SSD1306_Send_Data(Array[i]);
	}
}


void SSD1306_Fill(SSD1306_COLOR color)
{
	/* Set memory */
	volatile uint32_t i;

	for(i = 0; i < sizeof(SSD1306_Buffer); i++)
	{
		SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
	}
}

void ssd1306_UpdateScreen(void)
{
	volatile uint8_t i;
	for (i = 0; i < 8; i++) {
		SSD1306_Send_Command(0xB0 + i);
		SSD1306_Send_Command(0x00);
		SSD1306_Send_Command(0x10);
		SSD1306_Send_Command(0x40);
		//Fill OLED RAM by Buffer
		SSD1306_Send_Array(&SSD1306_Buffer[SSD1306_WIDTH * i],SSD1306_WIDTH);
	}
}

void ssd1306_DrawPixel(uint8_t x, uint8_t y,uint8_t color)
{
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
	{
		// We are not going to write outside the screen
		return;
	}
	// See if the pixel needs to be inverted
	if (Inverted)
	{
		color = (SSD1306_COLOR)!color;
	}
	// We set the right color for the pixel
	if (color == White)
	{
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	}
	else
	{
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

//
// We want to send 1 char to the screen
// ch => char to write
// Font => Font with which we are going to write
// color => Black or White

char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color)
{
	volatile uint32_t i, b, j;
	// See if there is still room on this line
	if (SSD1306_WIDTH  <= (SSD1306.CurrentX + Font.FontWidth ) ||
		  SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight))
	{
		// There is no more room
		return 0;
	}
	// We go through the font
	for (i = 0; i < Font.FontHeight; i++)
	{
		b = Font.data[(ch - 32) * Font.FontHeight + i];
		for (j = 0; j < Font.FontWidth; j++)
		{
			if ((b << j) & 0x8000)
			{
				ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
			}
			else
			{
				ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
			}
		}
	}
	// De huidige positie is nu verplaatst
	SSD1306.CurrentX += Font.FontWidth;
	// We geven het geschreven char terug voor validatie
	return ch;

}

//
//	Position the cursor
//
void ssd1306_SetCursor(uint8_t x, uint8_t y)
{
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

void ssd1306_drawLineH(uint8_t x0, uint8_t x1, uint8_t y, uint8_t step)
{
  if(step>1) { if(((x0&1)==1 && (y&1)==0) || ((x0&1)==0 && (y&1)==1)) x0++; }
  if(x1>x0) for(uint8_t x=x0; x<=x1; x+=step) ssd1306_DrawPixel(x,y,White);
  else      for(uint8_t x=x1; x<=x0; x+=step) ssd1306_DrawPixel(x,y,White);
}

void ssd1306_drawLineV(int x, int y0, int y1, int step)
{
  if(step>1) { if(((x&1)==1 && (y0&1)==0) || ((x&1)==0 && (y0&1)==1)) y0++; }
  if(y1>y0)for(int y=y0; y<=y1; y+=step) ssd1306_DrawPixel(x,y,1);
  else     for(int y=y1; y<=y0; y+=step) ssd1306_DrawPixel(x,y,1);
}

void ssd1306_print(char text[], FontDef Font, SSD1306_COLOR color){
	for(unsigned int i = 0; i<strlen(text);i++){
		ssd1306_WriteChar(text[i], Font, color);
	}
}

void ssd1306_drawCircle(uint8_t x0, uint8_t y0, uint8_t r, SSD1306_COLOR color){
#if defined(ESP8266)
    yield();
#endif
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    ssd1306_DrawPixel(x0  , y0+r, color);
    ssd1306_DrawPixel(x0  , y0-r, color);
    ssd1306_DrawPixel(x0+r, y0  , color);
    ssd1306_DrawPixel(x0-r, y0  , color);
    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        ssd1306_DrawPixel(x0 + x, y0 + y, color);
        ssd1306_DrawPixel(x0 - x, y0 + y, color);
        ssd1306_DrawPixel(x0 + x, y0 - y, color);
        ssd1306_DrawPixel(x0 - x, y0 - y, color);
        ssd1306_DrawPixel(x0 + y, y0 + x, color);
        ssd1306_DrawPixel(x0 - y, y0 + x, color);
        ssd1306_DrawPixel(x0 + y, y0 - x, color);
        ssd1306_DrawPixel(x0 - y, y0 - x, color);
    }
}
