/*
 * lcd.h
 *
 *  Created on: Dec 10, 2016
 *      Author: Matrim
 */

#ifndef LCD
#define LCD

typedef struct TSPoint
{
	int x;
	int y;
	int z;
} TSPoint;

// Pin Definitions
#define LCD_MOSI	0x0080		// data out at P1.7
#define LCD_SCK		0x0020		// serial clk at P1.5
#define LCD_RST 	0x0001		// reset at P2.0
#define LCD_CS		0x0002		// chip select at P2.1
#define LCD_DC		0x0004		// Data/Cmd at P2.2
#define TS_XP		0x0004		// X+ at p1.2
#define TS_XM		0x0001		// X- at p1.0 (Analog)
#define TS_YP		0x0002		// Y+ at p1.1 (Analog)
#define TS_YM		0x0008		// Y- at p1.3

// LCD pixel ranges
#define XPIXEL_MIN 0
#define XPIXEL_MAX 239
#define YPIXEL_MIN 0
#define YPIXEL_MAX 319


void initLCDScreen(void);
void calibrateScreen(float* , float*, int*, int*);
void writeLCDData(char);
void writeLCDControl(char);
void waitMS(unsigned int);
TSPoint getTSPoint(void);
void initClk(void);
void initPins(void);
void initLCD(void);
void initUSCI(void);

#endif /* LCD */
