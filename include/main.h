#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <omp.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#define MAX_POINTS 30
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640


void initializeTables(double sinTable[], double reverseSinTable[])
{
	for (int i = 0; i < 360; i++)
	{
		sinTable[i] = sin(i / 360.0 * 2 * PI) * -1;
		reverseSinTable[i] = sin(i / 360.0 * 2 * PI);
	}
}

#endif
