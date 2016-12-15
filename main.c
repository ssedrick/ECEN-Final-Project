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


TSPoint getJoystick(void);
int handleMenu(float*, float*, int*, int*);
int handleMove(short board[][3], int pixelX, int pixelY, int turn);
void start2PGame(float*, float*, int*, int*);
void drawX(int, int, int, int);
void drawBoard(void);
void endGame(void);


/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

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

	ADC10CTL0 = 0x1010;
	ADC10CTL1 = 0x4000;
	ADC10AE0 = JSX;
	ADC10CTL0 |= 0x0003;
	while (ADC10CTL1 & 0x0001);
	location.x = ADC10MEM;

	ADC10CTL0 = 0x1010;
	ADC10CTL1 = 0x6000;
	ADC10AE0 = JSY;
	ADC10CTL0 |= 0x0003;
	while (ADC10CTL1 & 0x0001);
	location.y = ADC10MEM;

	return location;
}

int handleMenu(float * ratioX, float * ratioY, int* xMin, int* yMin)
{
	TSPoint p, j;
	j.y = 500;
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
		//j = getJoystick();
		//b = !(P2IN & BTN1);

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
			if (j.y > 500)
			{
				if (!choice)
				{
					drawHorizontalLine(82, 158, 178, BLU);
					drawHorizontalLine(82, 158, 228, WHITE);
					choice = 1;

				}
			}
			else if (j.y < 450)
			{
				if (choice)
				{
					drawHorizontalLine(82, 158, 178, WHITE);
					drawHorizontalLine(82, 158, 228, BLU);
					choice = 0;
				}
			}
		}

	} while (!chosen);

	return choice;
}

void start2PGame(float* ratioX, float* ratioY, int* xMin, int* yMin)
{
	int turn=1, b=0, pixelX, pixelY;
	TSPoint underline, j, p;
	j.y = 470;
	j.x = 470;
	underline.x = 13;
	underline.y = 80;
	short board[3][3] = {0};
	fillScreen(WHITE);
	drawBoard();
	drawHorizontalLine(underline.x, underline.x + LENGTH, underline.y, RED);

	while (turn > 0)
	{
		waitMS(100);
		p = getTSPoint();
		//j = getJoystick();
		//b = !(P2IN & BTN1);

		if (p.z > 100)
		{
			pixelY = (p.y - *yMin) * (*ratioY);
			pixelX = (p.x - *xMin) * (*ratioX);
			turn = handleMove(board, pixelX, pixelY, turn);
			//TODO checkWin();
		}
		else if (b)
		{
			turn = handleMove(board, underline.x, underline.y, turn);
		}
		else
		{
			drawHorizontalLine(underline.x, underline.x + LENGTH, underline.y, WHITE);
			if (j.y > 600)
			{
				if (underline.y > 84)
					underline.y -= 74;
			}
			else if (j.y < 350)
			{
				if (underline.y < 156)
					underline.y += 74;
			}
			if (j.x > 600)
			{
				if (underline.x < 154)
					underline.x += 74;
			}
			else if (j.x < 350)
			{
				if (underline.x > 84)
					underline.x -= 74;
			}
			drawHorizontalLine(underline.x, underline.x + LENGTH, underline.y, RED);
		}
	}
}

int handleMove(short board[][3], int pixelX, int pixelY, int turn)
{
	unsigned int iX = pixelX / 74;
	unsigned int iY = (pixelY - 40) / 74;
	iY = (iY > 3 ? 3 : iY);
	if (!(board[iY][iX]))
	{
		board[iY][iX] = turn;
		iY = iY * 74 + 47;
		iX = iX * 74 + 47;
		if (turn == 1)
			drawX(iX, iY, 30, GRN);
		else
			drawCircle(iX, iY, 25, RED);
		turn = (turn == 1 ? 2 : 1);
	}

	return turn;
}

void drawBoard(void)
{
	drawHorizontalLine(10, 230, 84, BLK);
	drawHorizontalLine(10, 230, 156, BLK);
	drawVerticalLine(84, 10, 230, BLK);
	drawVerticalLine(156, 10, 230, BLK);
}

void drawX(int x, int y, int size, int color)
{
	size = size / 2;
	drawLine(x - size, y - size, x + size, y + size, color);
	drawLine(x - size, y + size, x + size, y - size, color);
}

void endGame(void)
{
	fillScreen(BLK);
	drawWord("Thanks for playing!", 19, 45, 120, WHITE);
}
