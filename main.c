#include <main.h>

double WAVE_AMPLITUDE = 50.0;
double WAVE_SPEED = 0.01;
double WAVE_SPEED_2 = -0.01;
double WAVE_PHASE = 0.0;
double WAVE_PHASE_2 = 0.0;
double WAVE_INTENSITY = 1.0;

typedef struct
{
	double x;
	double y;
	int type;
} Point;

double my_time = 100.0;
double reverseSinTable[360] = {0};
double sinTable[360] = {0};
int waveArray[SCREEN_WIDTH][SCREEN_HEIGHT];
Point pointsIn[MAX_POINTS];
Point pointsOut[MAX_POINTS];
int pointCountIn = 0;
int pointCountout = 0;
void newPoint(Point *pointsIn, Point *pointsOut,
			  int *pointCountIn, int *pointCountout)
{
	Rectangle Sliders = {0, 0, 400, 120};

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		if (*pointCountIn < MAX_POINTS)
		{
			int mouseX = GetMouseX();
			int mouseY = GetMouseY();
			if (!CheckCollisionPointRec((Vector2){mouseX, mouseY}, Sliders))
			{
				pointsIn[*pointCountIn].x = mouseX;
				pointsIn[*pointCountIn].y = mouseY;
				(*pointCountIn)++;
			}
		}
		else
		{
			printf("Maximum number of points reached.\n");
		}
	}

	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
	{
		if (*pointCountout < MAX_POINTS)
		{
			int mouseX = GetMouseX();
			int mouseY = GetMouseY();
			if (!CheckCollisionPointRec((Vector2){mouseX, mouseY}, Sliders))
			{
				pointsOut[*pointCountout].x = mouseX;
				pointsOut[*pointCountout].y = mouseY;
				(*pointCountout)++;
			}
		}
		else
		{
			printf("Maximum number of points reached.\n");
		}
	}
}

void drawWave(int waveArray[][SCREEN_HEIGHT])
{
	BeginDrawing();
	//ClearBackground(WHITE);
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			Color color = (Color){waveArray[i][j] + i + j, waveArray[i][j]+ i - j, waveArray[i][j] - i - j, 255};
			DrawPixel(i, j, color);
		}
	}
	WAVE_AMPLITUDE = GuiSliderBar((Rectangle){0, 0, 400, 20}, NULL, "Wave Amplitude", WAVE_AMPLITUDE, 0, 50);
	WAVE_SPEED = GuiSliderBar((Rectangle){0, 20, 400, 20}, NULL, "Wave Speed", WAVE_SPEED, -0.1, 0.1);
	WAVE_SPEED_2 = GuiSliderBar((Rectangle){0, 40, 400, 20}, NULL, "Wave Speed 2", WAVE_SPEED_2, -0.1, 0.1);
	WAVE_PHASE = GuiSliderBar((Rectangle){0, 60, 400, 20}, NULL, "Wave Phase", WAVE_PHASE, -M_PI, M_PI);
	WAVE_PHASE_2 = GuiSliderBar((Rectangle){0, 80, 400, 20}, NULL, "Wave Phase 2", WAVE_PHASE_2, -M_PI, M_PI);
	WAVE_INTENSITY = GuiSliderBar((Rectangle){0, 100, 400, 20}, NULL, "Wave intensity", WAVE_INTENSITY, 0.00001, 20);
	EndDrawing();
}

void calculateWaveIntensities(Point pointsIn[], Point pointsOut[],
							  int pointCountIn, int pointCountOut,
							  double sinTable[], double timeLeft,
							  double timeRight, int blockStart,
							  int blockEnd, int waveArray[][SCREEN_HEIGHT])
{
	for (int i = blockStart; i < blockEnd; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			double sum1 = 0.0;
			double sum2 = 0.0;
			for (int k = 0; k < pointCountIn; k++)
			{
				double dist = sqrt(pow(i - pointsIn[k].x, 2) + pow(j - pointsIn[k].y, 2));
				double intensity = sinTable[(int)((dist / WAVE_AMPLITUDE - timeLeft) * 360) % 360];
				sum1 += intensity / WAVE_INTENSITY;
			}
			for (int k = 0; k < pointCountOut; k++)
			{
				double dist = sqrt(pow(i - pointsOut[k].x, 2) + pow(j - pointsOut[k].y, 2));
				double intensity = sinTable[(int)((dist / WAVE_AMPLITUDE - timeRight) * 360) % 360];
				sum2 += intensity / WAVE_INTENSITY;
			}
			double sum = (sum1 + sum2);
			double sigmoidValue = 1 / (1 + exp(-sum));
			waveArray[i][j] = (int)(sigmoidValue * 255);
		}
	}
}



#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

void UpdateDrawFrame(void)
{
	newPoint(pointsIn, pointsOut, &pointCountIn, &pointCountout);
//	int numThreads = omp_get_max_threads();
	int numThreads = 1;
	int blockWidth = SCREEN_WIDTH / numThreads;
//#pragma omp parallel
		{
		//int threadId = omp_get_thread_num();
		int threadId = 0;
		int blockStart = threadId * blockWidth;
		int blockEnd = (threadId == numThreads - 1) ? SCREEN_WIDTH : (threadId + 1) * blockWidth;
		double timeLeft = my_time;
		double timeRight = my_time * WAVE_SPEED_2 / WAVE_SPEED;
		calculateWaveIntensities(pointsIn, pointsOut,
								 pointCountIn, pointCountout,
								 sinTable, timeLeft, timeRight,
								 blockStart, blockEnd, waveArray);
		}
	my_time += WAVE_SPEED;
	drawWave(waveArray);
}

int main(void)
{
	//GuiLoadStyle("resources/raylib/raygui_dark.rgs");
	GuiSetFont(GetFontDefault());
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "wave generator");

	initializeTables(sinTable, reverseSinTable); // <- new
	#if defined(PLATFORM_WEB)
  		emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
	#else
    	SetTargetFPS(60);
    	while (!WindowShouldClose())
        	UpdateDrawFrame();
	#endif
	/*
	SetTargetFPS(60);
	simulationLoop(pointsIn, pointsOut,
				   pointCountIn, pointCountout, waveArray);
	*/
	CloseWindow();
	return 0;
}

