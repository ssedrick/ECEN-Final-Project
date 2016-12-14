/*
 * draw.c
 *
 *  Created on: Dec 10, 2016
 *      Author: Matrim
 */

#include "draw.h"
#include "lcd.h"
#include "fonts.h"

void fillScreen(int color)
{
	unsigned int i;

	writeLCDControl(0x2A);				// Select Column Address
	writeLCDData(0);					// Setup beginning column address
	writeLCDData(0);					// Setup beginning column address
	writeLCDData(0);					// Setup ending column address
	writeLCDData(239);					// Setup ending column address

	writeLCDControl(0x2B);				// Select Row Address
	writeLCDData(0);					// Setup beginning row address
	writeLCDData(0);					// Setup beginning row address
	writeLCDData(0x01);					// Setup ending row address
	writeLCDData(0x3F);					// Setup ending row address

	writeLCDControl(0x2C);				// Select Memory Write
	for (i = 38401 ; i > 0 ; i--)		// Loop through all memory locations 16 bit color
	{
		writeLCDData(color >> 8);		// Write data to LCD memory
		writeLCDData(color & 0xff);		// Write data to LCD memory
		writeLCDData(color >> 8);		// Write data to LCD memory
		writeLCDData(color & 0xff);		// Write data to LCD memory
	}
}

void drawPixel(int x, int y, int color)
{
	writeLCDControl(0x2A);				// Select Column Address
	writeLCDData(x >> 8);				// Starting x Address (Most Sig 8 bits of address)
	writeLCDData(x & 0xff);				// Starting x Address (Least Sig 8 bits of address)
	writeLCDData(x >> 8);				// Ending x Address (Most Sig 8 bits of address)
	writeLCDData(x & 0xff);				// Ending x Address (Least Sig 8 bits of address)

	writeLCDControl(0x2B);				// Select Row Address
	writeLCDData(y >> 8);				// Starting y Address (Most Sig 8 bits of address)
	writeLCDData(y & 0xff);				// Starting y Address (Least Sig 8 bits of address)
	writeLCDData(y >> 8);				// Ending y Address (Most Sig 8 bits of address)
	writeLCDData(y & 0xff);				// Ending y Address (Least Sig 8 bits of address)

	writeLCDControl(0x2C);				// Select Color to write to the pixel
	writeLCDData(color >> 8);			// Send Most Significant 8 bits of 16 bit color
	writeLCDData(color & 0xff);			// Send Least Significant 8 bits of 16 bit color
}

void drawHorizontalLine(int x1, int x2, int y, int color)
{
	for (; x1 <= x2; x1++)
		drawPixel(x1, y, color);
}

void drawVerticalLine(int x, int y1, int y2, int color)
{
    for (; y1 <= y2; y1++)
    	drawPixel(x, y1, color);
}

void drawLine(int x1, int y1, int x2, int y2, int color)
{
    int error, deltaX, deltaY, yStep, steep;

    if(((y2 > y1) ? (y2 - y1) : (y1 - y2)) >
       ((x2 > x1) ? (x2 - x1) : (x1 - x2)))
        steep = 1;
    else
        steep = 0;

    //
    // If the line is steep, then swap the X and Y coordinates.
    //
    if(steep)
    {
        error = x1;
        x1 = y1;
        y1 = error;
        error = x2;
        x2 = y2;
        y2 = error;
    }

    //
    // If the starting X coordinate is larger than the ending X coordinate,
    // then swap the start and end coordinates.
    //
    if(x1 > x2)
    {
        error = x1;
        x1 = x2;
        x2 = error;
        error = y1;
        y1 = y2;
        y2 = error;
    }

    //
    // Compute the difference between the start and end coordinates in each
    // axis.
    //
    deltaX = x2 - x1;
    deltaY = (y2 > y1) ? (y2 - y1) : (y1 - y2);

    //
    // Initialize the error term to negative half the X delta.
    //
    error = -deltaX / 2;

    //
    // Determine the direction to step in the Y axis when required.
    //
    if(y1 < y2)
        yStep = 1;
    else
        yStep = -1;

    //
    // Loop through all the points along the X axis of the line.
    //
    for(; x1 <= x2; x1++)
    {
        if(steep)
        	drawPixel(y1, x1, color);
        else
        	drawPixel(x1, y1, color);

        error += deltaY;

        if(error > 0)
        {
            y1 += yStep;
            error -= deltaX;
        }
    }
}


void drawLetter(char l, int x, int y, int color)
{
	const char * letter = font[l];
	int i;
	int j;
	const unsigned char mask = 0x80;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if ((letter[i] & (mask >> j))) {
				drawPixel(x + j, y + i, color);
			}
		}
	}
}


void drawWord(char word[], int size, int x, int y, int color)
{
	int i = size;
	for (; i > 0; i--)
	{
		drawLetter(word[size - i], x, y, color);
		x += 8;
	}
}

void drawColorWord(char word[], int size, int x, int y, int color[])
{
	int i = size;
	for (; i > 0; i--)
	{
		drawLetter(word[size - i], x, y, color[size - i]);
		x += 8;
	}
}

void drawCircle(int x0, int y0, int radius, int color)
{
	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y)
	{
		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 - y, y0 - x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 + x, y0 - y, color);

		y += 1;
		err += 1 + 2 * y;
		if (2 * (err - x) + 1 > 0)
		{
			x -= 1;
			err += 1 - 2 * x;
		}
	}
}

void drawRectangle(int x, int y, int width, int height, int color)
{
	int i, j;
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			drawPixel(x + i, y + j, color);
		}
	}
}

void drawButton(char word[], int size, int x, int y, int width, int height, int colorBtn, int colorWord)
{
	int recX = x - (width >> 1);
	int recY = y - (height >> 1);
	int wordX = x - ((size >> 1) << 3);
	if (size % 2)
		wordX -= 4;
	int wordY = y - 4;
	drawRectangle(recX, recY, width, height, colorBtn);
	drawWord(word, size, wordX, wordY, colorWord);
}
