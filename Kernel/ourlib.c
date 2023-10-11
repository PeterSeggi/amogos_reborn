#include <ourlib.h>

static uint8_t * currentVideo = (uint8_t*)0xB8000;


void ourPrintCharColor(char character, char color)
{
	*currentVideo = character;
	currentVideo++;
	*currentVideo = color;
	currentVideo++;
}

void ourPrintColor(const char * string, char color)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ourPrintCharColor(string[i],color);
}
