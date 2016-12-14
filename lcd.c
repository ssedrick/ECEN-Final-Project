/*
 * lcd.c
 *
 *  Created on: Dec 10, 2016
 *      Author: Matrim
 */
#include <msp430g2553.h>
#include "lcd.h"
#include "fonts.h"
#include "draw.h"

void initLCDScreen(void)
{
	initClk();							// Init clock to 16 MHz
	initPins();							// Init Pin Functionality
	initUSCI();							// Init USCI (SPI)
  	P2OUT &= ~LCD_RST;
  	waitMS(500);
  	P2OUT = LCD_RST;
  	initLCD();
}

void calibrateScreen(float* ratioX, float* ratioY, int* xMin, int* yMin)
{
	TSPoint p;
	int xMax, yMax;
	fillScreen(BLK);

	drawWord("First touch the top left,", 25, 25, 100, WHITE);
	drawWord("then touch the bottom", 21, 25, 110, WHITE);
	drawWord("right of the screen.", 20, 25, 120, WHITE);

	int touchCount = 0;
	while (touchCount < 2)
	{
		waitMS(10);
		p = getTSPoint();

		if (p.z > 50 && !touchCount)
		{
			*xMin = p.x;
			*yMin = p.y;
			touchCount++;
			while (p.z > 50)
			{
				waitMS(10);
				p = getTSPoint();
			}
		}
		else if (p.z > 50 && touchCount)
		{
			xMax = p.x;
			yMax = p.y;
			touchCount++;
			while (p.z > 50)
			{
				waitMS(10);
				p = getTSPoint();
			}
		}
	}

	*ratioX = ((float)XPIXEL_MAX) / (xMax - *xMin);
	*ratioY = ((float)YPIXEL_MAX) / (yMax - *yMin);
}

void initClk(void)
{
   BCSCTL1 = CALBC1_16MHZ;				// 16 MHz Operation
   DCOCTL = CALDCO_16MHZ;
}

void initPins(void)
{

	P1DIR = LCD_MOSI | LCD_SCK | TS_XP | TS_YM;	// set out put pins for P1
	P2DIR = LCD_RST | LCD_CS | LCD_DC;			// set out put pins for P2

	P1SEL |= LCD_MOSI + LCD_SCK;
	P1SEL2 |= LCD_MOSI + LCD_SCK;

	ADC10CTL0 = 0x1010; // Sample/Hold time and ADC on
	ADC10AE0 = TS_XM | TS_YP; // P1.0,P1.1 ADC option select

	P2OUT = LCD_RST | LCD_CS | LCD_DC;
}

void writeLCDControl(char data)
{
	P2OUT &= ~LCD_DC;					// DC low
	P2OUT &= ~LCD_CS;					// CS Low

  	while(IFG2&UCB0TXIFG);				// TX buffer ready?
  	UCB0TXBUF=data;						// start transmission

    return;
}

void writeLCDData(char data)
{
	P2OUT |= LCD_DC;					// DC high
	P2OUT &= ~LCD_CS;					// CS Low

	while(IFG2&UCB0TXIFG);				// TX buffer ready?
  	UCB0TXBUF=data;						// start transmission

    return;
}

void waitMS(unsigned int m_sec)
{
	while(m_sec--)
		__delay_cycles(16000);			// 1000 for 1MHz
}

void initLCD(void)
{
    P1OUT = 0x00;						// Set all outputs low for initialization
    P2OUT = LCD_RST | LCD_CS | LCD_DC;

	P2OUT |= LCD_RST;					// Reset LCD
	waitMS(10);
	P2OUT &= ~LCD_RST;
	waitMS(10);
	P2OUT |= LCD_RST;
	waitMS(100);

	writeLCDControl(0xEF);				//
	writeLCDData(0x03);
	writeLCDData(0x80);
	writeLCDData(0x02);

	writeLCDControl(0xCF);				// Power Control B
	writeLCDData(0x00);
	writeLCDData(0xC1);
	writeLCDData(0x30);

	writeLCDControl(0xED);				// Power On Sequence Control
	writeLCDData(0x64);
	writeLCDData(0x03);
	writeLCDData(0x12);
	writeLCDData(0x81);

	writeLCDControl(0xE8);				// Driver Timing Control A
	writeLCDData(0x85);
	writeLCDData(0x00);
	writeLCDData(0x78);

	writeLCDControl(0xCB);				// Power Control A
	writeLCDData(0x39);
	writeLCDData(0x2C);
	writeLCDData(0x00);
	writeLCDData(0x34);
	writeLCDData(0x02);

	writeLCDControl(0xF7);				// Pump Ration Control
	writeLCDData(0x20);

	writeLCDControl(0xEA);				// Driver Timing Control A
	writeLCDData(0x00);
	writeLCDData(0x00);

	writeLCDControl(0xC0); 				// Power Control 1
	writeLCDData(0x23);

	writeLCDControl(0xC1);    			// Power Control 2
	writeLCDData(0x10);

	writeLCDControl(0xC5);    			// VCOM Control
	writeLCDData(0x3e);
	writeLCDData(0x28);

	writeLCDControl(0xC7);    			// VCOM Control2
	writeLCDData(0x86);

	writeLCDControl(0x36);    			// Memory Access Control
	writeLCDData(0x48);

	writeLCDControl(0x3A);				// COLMOD Pixel Format Set
	writeLCDData(0x55);

	writeLCDControl(0xB1);				// Frame Rate Control
	writeLCDData(0x00);
	writeLCDData(0x18);

	writeLCDControl(0xB6);    			// Display Function Control
	writeLCDData(0x08);
	writeLCDData(0x82);
	writeLCDData(0x27);

	writeLCDControl(0xF2);    			// 3Gamma Control
	writeLCDData(0x00);

	writeLCDControl(0x26);    			// Gamma curve selected
	writeLCDData(0x01);

	writeLCDControl(0xE0);   			// Positive Gamma Correction
	writeLCDData(0x0F);
	writeLCDData(0x31);
	writeLCDData(0x2B);
	writeLCDData(0x0C);
	writeLCDData(0x0E);
	writeLCDData(0x08);
	writeLCDData(0x4E);
	writeLCDData(0xF1);
	writeLCDData(0x37);
	writeLCDData(0x07);
	writeLCDData(0x10);
	writeLCDData(0x03);
	writeLCDData(0x0E);
	writeLCDData(0x09);
	writeLCDData(0x00);

	writeLCDControl(0xE1);    			// Negative Gamma Correction
	writeLCDData(0x00);
	writeLCDData(0x0E);
	writeLCDData(0x14);
	writeLCDData(0x03);
	writeLCDData(0x11);
	writeLCDData(0x07);
	writeLCDData(0x31);
	writeLCDData(0xC1);
	writeLCDData(0x48);
	writeLCDData(0x08);
	writeLCDData(0x0F);
	writeLCDData(0x0C);
	writeLCDData(0x31);
	writeLCDData(0x36);
	writeLCDData(0x0F);

	writeLCDControl(0x11);    			// Exit Sleep
	writeLCDControl(0x29);    			// Display on
}

void initUSCI(void)
{
  	UCB0CTL1|=UCSWRST;					// USCI in reset state
  	UCB0CTL0|=UCMST+UCSYNC+UCCKPH+UCMSB;// SPI Master, 8bit, MSB first, synchronous mode
  	UCB0CTL1|=UCSSEL_2;					// USCI CLK-SRC=SMCLK=~8MHz
  	UCB0CTL1&=~UCSWRST;					// USCI released for operation
  	IE2|=UCB0TXIE;						// enable TX interrupt
  	IFG2&=~UCB0TXIFG;

  	_EINT();							// enable interrupts
}

TSPoint getTSPoint(void)
{
	int x=0, y=0, z=0;
    int sample;
    TSPoint location;

    //
    // Code to read the X position on the touchscreen
    //
    P1DIR &= ~TS_YP;           // Y+ set to input
    P1DIR &= ~TS_YM;           // Y- set to input
    P1DIR |= TS_XP;                   // X+ set to output
    P1DIR |= TS_XM;                   // X- set to output

    P1OUT |= TS_XP;                   // X+ set to high
    P1OUT &= ~TS_XM;           // X- set to low

    ADC10CTL0 = 0x1010; // Sampling and conversion start
    ADC10CTL1 = 0x1000; // Input A1 into ADC
    ADC10AE0 = TS_YP;          // P1.1 ADC option select
    ADC10CTL0 |= 0x0003;       // Sampling and conversion start
    while (ADC10CTL1 & 0x0001); // Wait for conversion to complete
    sample = ADC10MEM;         // Read ADC value
    x = (1023 - sample); // Set top left corner = 0

    //
    // Code to read the Y position on the touchscreen
    //
    P1DIR &= ~TS_XP;           // X+ set to input
    P1DIR &= ~TS_XM;           // X- set to input
    P1DIR |= TS_YP;                   // Y+ set to output
    P1DIR |= TS_YM;                   // Y- set to output

    P1OUT |= TS_YP;                   // Y+ out set high
    P1OUT &= ~TS_YM;           // Y- out set low

    ADC10CTL0 = 0x1010; // Sampling and conversion start
    ADC10CTL1 = 0x0000; // Input A0(X-) into ADC
    ADC10AE0 = TS_XM;          // P1.0 ADC option select
    ADC10CTL0 |= 0x0003;       // Sampling and conversion start
    while (ADC10CTL1 & 0x0001); // Wait for conversion to complete
    sample = ADC10MEM;         // Read ADC value
    y = (1023 - sample); // Set top left corner = 0

    //
    // Code to read the Z position on the touchscreen
    //
    P1DIR |= TS_YM;                   // Y- set to output
    P1DIR &= ~TS_YP;           // Y+ set to input
    P1DIR &= ~TS_XM;           // X- set to input
    P1DIR |= TS_XP;                   // X+ set to output

    P1OUT |= TS_YM;                   // Set Y- high
    P1OUT &= ~TS_XP;           // Set X+ low

    ADC10CTL0 = 0x1010; // Sampling and conversion start
    ADC10CTL1 = 0x0000; // Input A0(X-) into ADC
    ADC10AE0 = TS_XM;          // P1.0 ADC option select
    ADC10CTL0 |= 0x0003;       // Sampling and conversion start
    while (ADC10CTL1 & 0x0001); // Wait for conversion to complete
    int z1= ADC10MEM;          // Read ADC value

    ADC10CTL0 = 0x1010; // Sampling and conversion start
    ADC10CTL1 = 0x1000; // Input A1(Y+) into ADC
    ADC10AE0 = TS_YP;          // P1.1 ADC option select
    ADC10CTL0 |= 0x0003;       // Sampling and conversion start
    while (ADC10CTL1 & 0x0001); // Wait for conversion to complete
    int z2= ADC10MEM;          // Read ADC value

    z = (1023-(z2-z1));

    location.x = x;
    location.y = y;
    location.z = z;

    return location;
}

#pragma INTERRUPT (USCI)
#pragma vector=USCIAB0TX_VECTOR
void USCI(void)
{
	P2OUT|=LCD_CS;						// transmission done
	IFG2&=~UCB0TXIFG;					// clear TXIFG
}
