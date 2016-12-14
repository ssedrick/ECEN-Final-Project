/*
 * draw.h
 *
 *  Created on: Dec 10, 2016
 *      Author: Matrim
 */

#ifndef DRAW
#define DRAW

// Function Prototypes
void fillScreen(int);
void drawPixel(int, int, int);
void drawHorizontalLine(int, int, int, int);
void drawVerticalLine(int, int, int, int);
void drawLine(int, int, int, int, int);
void drawLetter(char, int, int, int);
void drawWord(char[], int, int, int, int);
void drawColorWord(char[], int, int, int, int[]);
void drawRectangle(int, int, int, int, int);
void drawCircle(int, int, int, int);
void drawButton(char[], int, int, int, int, int, int, int);

#endif /* DRAW */
