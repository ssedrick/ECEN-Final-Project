#include <msp430g2553.h>
#include "fonts.h"
#include "draw.h"
#include "lcd.h"

#define LENGTH 68
#define BTN1 0x0020      // P2.5
#define BTN2 0x0010      // P2.4
#define BTN3 0x0008      // P2.3
#define JSY  0x0040      // P1.6
#define JSX  0x0010      // P1.4

int adc[7] = {0};

TSPoint getJoystick(void);
void initJoystick(void);
int handleMenu(float*, float*, int*, int*);
void start2PGame(float*, float*, int*, int*);
void drawBoard(void);
void endGame(void);


/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    initJoystick();

    P2DIR |= 0x0000;
    P2OUT |= BTN1 | BTN2 | BTN3;   // Set up button

	initLCDScreen();

    float ratioX, ratioY;
    int xMin, yMin;

    calibrateScreen(&ratioX, &ratioY, &xMin, &yMin);

  	fillScreen(WHITE);

  	int choice = -1;

  	do
  	{
		choice = handleMenu(&ratioX, &ratioY, &xMin, &yMin);

		switch (choice)
		{
			case 1:
				start2PGame(&ratioX, &ratioY, &xMin, &yMin);
				break;
			//case 2:
			//	start2PGame();
			//	break;
			case 0:
				break;
			default:
				break;
		}
  	} while (choice);

  	endGame();
	
	return 0;
}

TSPoint getJoystick(void)
{
	TSPoint location;

	ADC10CTL0 &= ~ENC;
	while (ADC10CTL1 & BUSY);
	ADC10SA = (unsigned)&adc[0]; //RAM Address of ADC Data, must be reset every conversion
	ADC10CTL0 |= (ENC | ADC10SC); //Start ADC Conversion
	while (ADC10CTL1 & BUSY);

	location.x = adc[4];
	location.y = adc[6];

	return location;
}

void initJoystick(void)
{
	ADC10CTL1 = INCH_6 | CONSEQ_1; // A4 + A3 + A2 + A1, single sequence
	ADC10CTL0 = ADC10SHT_2 | MSC | ADC10ON;
	while (ADC10CTL1 & BUSY);
	ADC10DTC1 = 0x07; // 4 conversions
	ADC10AE0 |= 0x7F; // ADC10 option select
}

int handleMenu(float * ratioX, float * ratioY, int* xMin, int* yMin)
{
	TSPoint p, j;
	int chosen=0, choice=1, b=0, pixelY;
	// TODO drawButton("1P Game", 7, , , 70, 30, RED, WHITE);
	drawButton("Play", 4, 120, 160, 70, 30, RED, WHITE);
	drawHorizontalLine(82, 158, 178, BLU);
	drawButton("Quit", 4, 120, 210, 70, 30, RED, WHITE);

	drawVerticalLine(100, 10, 110, BLU);
	drawVerticalLine(140, 10, 110, BLU);
	drawHorizontalLine( 70, 170, 43, BLU);
	drawHorizontalLine( 70, 170, 76, BLU);

	do
	{
		waitMS(10);
		p = getTSPoint();
		j = getJoystick();
		//j.x = P1IN & JSX;
		//j.y = P1IN & JSY;
		b = !(P2IN & BTN1);

		if (p.z > 100)
		{
			pixelY = (p.y - *yMin) * (*ratioY);
			if (pixelY > 135 && pixelY < 185)
				chosen = choice = 1;
			else if (pixelY > 185 && pixelY < 235)
			{
				choice = 0;
				chosen = 1;
			}
		}
		else if (b)
		{
			chosen = 1;
		}
		else
		{
			j;
		}

	} while (!chosen);

	return choice;
}

void start2PGame(float* ratioX, float* ratioY, int* xMin, int* yMin)
{
	int turn = 1;
	TSPoint underline;
	underline.x = 13;
	underline.y = 80;
	fillScreen(WHITE);
	drawBoard();
	drawHorizontalLine(underline.x, underline.x + LENGTH, underline.y, RED);

	while (turn > 0)
	{

	}
}

void drawBoard(void)
{
	drawHorizontalLine(10, 230, 84, BLK);
	drawHorizontalLine(10, 230, 156, BLK);
	drawVerticalLine(84, 10, 230, BLK);
	drawVerticalLine(156, 10, 230, BLK);
}

void endGame(void)
{
	fillScreen(BLK);
	drawWord("Thanks for playing!", 19, 45, 120, WHITE);
}
