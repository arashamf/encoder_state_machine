#ifndef OLED_H
#define OLED_H

//Includes ------------------------------------------------------------------//
#include "main.h"
#include "OLED_Fonts.h"
#include "OLED_Icons.h"

//Private defines -----------------------------------------------------------//
//#define OLED_I2C_ADDRESS									0x78	
#define OLED_WIDTH						                128
#define OLED_HEIGHT						                64
#define OLED_BUFFERSIZE						            (OLED_WIDTH * OLED_HEIGHT)/8
#define NONE_INVERTED						            0x00
#define INVERTED						                0x01
#define NUMB_PAGE                                       8

// Fundamental Commands
#define OLED_SET_CONTRAST_CONTROL				        0x81
#define OLED_DISPLAY_ALL_ON_RESUME				        0xA4
#define OLED_DISPLAY_ALL_ON					            0xA5
#define OLED_NORMAL_DISPLAY					            0xA6
#define OLED_INVERT_DISPLAY					            0xA7
#define OLED_DISPLAY_OFF					            0xAE
#define OLED_DISPLAY_ON						            0xAF
#define OLED_NOP						                0xE3

// Scrolling Commands
#define OLED_HORIZONTAL_SCROLL_RIGHT				    0x26
#define OLED_HORIZONTAL_SCROLL_LEFT				        0x27
#define OLED_HORIZONTAL_SCROLL_VERTICAL_AND_RIGHT		0x29
#define OLED_HORIZONTAL_SCROLL_VERTICAL_AND_LEFT		0x2A
#define OLED_DEACTIVATE_SCROLL					        0x2E
#define OLED_ACTIVATE_SCROLL					        0x2F
#define OLED_SET_VERTICAL_SCROLL_AREA				    0xA3
// Addressing Setting Commands
#define OLED_SET_LOWER_COLUMN					        0x00
#define OLED_SET_HIGHER_COLUMN					        0x10
#define OLED_MEMORY_ADDR_MODE					        0x20
#define OLED_SET_COLUMN_ADDR					        0x21
#define OLED_SET_PAGE_ADDR					            0x22
// Hardware Configuration Commands
#define OLED_SET_START_LINE					            0x40
#define OLED_SET_SEGMENT_REMAP					        0xA0
#define OLED_SET_MULTIPLEX_RATIO				        0xA8
#define OLED_COM_SCAN_DIR_INC					        0xC0
#define OLED_COM_SCAN_DIR_DEC					        0xC8
#define OLED_SET_DISPLAY_OFFSET					        0xD3
#define OLED_SET_COM_PINS					            0xDA
#define OLED_CHARGE_PUMP					            0x8D
// Timing & Driving Scheme Setting Commands
#define OLED_SET_DISPLAY_CLOCK_DIV_RATIO			    0xD5
#define OLED_SET_PRECHARGE_PERIOD				        0xD9
#define OLED_SET_VCOM_DESELECT					        0xDB

//Prototypes---------------------------------------------------------------//
void OLED_ClearBuffer(uint8_t value);
void OLED_SendCommand(uint8_t command);
void OLED_SendData(uint8_t data);
uint8_t OLED_init(void);
void OLED_Clear(uint8_t mode);
void OLED_Invert(uint8_t inverted);
void OLED_SetContrast(uint8_t contrast);
uint8_t OLED_GetWidthStr(const char* str);
uint8_t OLED_GetWidthNum(int16_t num);
void OLED_DrawNumSetMode(int16_t , uint8_t , uint8_t , uint8_t );
void OLED_DrawNum(int16_t , uint8_t , uint8_t );
void OLED_DrawStrSetMode(const char *, uint8_t , uint8_t , uint8_t );
void OLED_DrawCharSetMode(char , uint8_t , uint8_t , uint8_t );
void OLED_DrawStr(const char * , uint8_t , uint8_t );
void OLED_DrawChar(char , uint8_t , uint8_t );
void OLED_DrawBitmap(const uint8_t *, uint8_t, uint8_t , uint8_t , uint8_t) ;
void OLED_DrawBuffer(const uint8_t *progmem_buffer);
void OLED_DrawPixelStatus(uint8_t pos_x, uint8_t pos_y, uint8_t mode);
void OLED_DrawPixel(uint8_t pos_x, uint8_t pos_y);
void OLED_DrawXBM(int16_t xMove, int16_t yMove, const uint8_t *xbm);
void OLED_DrawVLine(uint8_t x, uint8_t y, uint8_t length);
void OLED_DrawHLine(uint8_t x, uint8_t y, uint8_t length, uint8_t mode);
void OLED_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void OLED_DrawRectangleFill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode);
void OLED_DrawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3) ;
void OLED_DrawTriangleFill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3) ;
void OLED_DrawCircle(int16_t x0, int16_t y0, uint8_t r);
void OLED_DrawCircleFill(int16_t x0, int16_t y0, int16_t r);
void OLED_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
void OLED_UpdateScreen(void);
void OLED_UpdateOnePage(uint8_t page_number);


#endif
