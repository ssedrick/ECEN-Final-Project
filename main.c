#include <msp430g2553.h>
#include "fonts.h"
#include "draw.h"
#include "lcd.h"

#define LENGTH 68

int handleMenu(float*, float*, int*, int*);
void start2PGame(float*, float*, int*, int*);
void drawBoard(void);
void endGame(void);

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    P2OUT |= 0x30;   // Set up button

    initLCDScreen();

    float ratioX, ratioY;
    int xMin, yMin;

    // calibrateScreen(&ratioX, &ratioY, &xMin, &yMin);

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

int handleMenu(float * ratioX, float * ratioY, int* xMin, int* yMin)
{
	TSPoint p;
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
		// j = getJoystick();
		b = 1;

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
		else if (!(P2OUT & 0x30))
		{
			choice = chosen = 1;
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
