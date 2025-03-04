#include "i2c.h"
#include "fonts.h"
//Defines--------------------------------
#define SSD1306_Address 0b111100
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 32
#define SSD1306_BUFFERSIZE (SSD1306_WIDTH*SSD1306_HEIGHT)/8
//---------------------------------------

typedef enum {
	Black = 0x00, /*!< Black color, no pixel */
	White = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR;

//
//  Struct to store transformations
//
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;

void SSD1306_Init(void);
void SSD1306_Send_Command(char command);
void SSD1306_Send_Data(char data);
void SSD1306_Send_Array(char *Array, int size);
void SSD1306_Fill(SSD1306_COLOR c);
void ssd1306_UpdateScreen(void);
void ssd1306_DrawPixel(uint8_t, uint8_t,uint8_t);
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
void ssd1306_SetCursor(uint8_t x, uint8_t y);
void ssd1306_drawLineH(uint8_t x0, uint8_t x1, uint8_t y, uint8_t step);
void ssd1306_drawLineV(int x, int y0, int y1, int step);
void ssd1306_print(char text[], FontDef Font, SSD1306_COLOR color);
void ssd1306_drawCircle(uint8_t x0, uint8_t y0, uint8_t r, SSD1306_COLOR color);
