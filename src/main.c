#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "epub.h"

static int update(void *userdata);
const char *fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont *font = NULL;

#ifdef _WINDLL
__declspec(dllexport)
#endif

	static PlaydateAPI *pd = NULL;

	size_t top_of_stack;

	int32_t thing[32768];

int eventHandler(PlaydateAPI *playdate, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if (event == kEventInit)
	{
		pd = playdate;
		const char *err;
		font = pd->graphics->loadFont(fontpath, &err);
		//pd->display->setRefreshRate(0);

		if (font == NULL)
			pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);

		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
		pd->system->setUpdateCallback(update, pd);

		InitEpub(playdate);
		readStuff();


	}

	return 0;
}

#define TEXT_WIDTH 86
#define TEXT_HEIGHT 16

int x = (400 - TEXT_WIDTH) / 2;
int y = (240 - TEXT_HEIGHT) / 2;
int dx = 5;
int dy = 7;

static int update(void *userdata)
{
	// unzip()
	PlaydateAPI *pd = userdata;
	
	pd->graphics->fillRect(x, y,TEXT_WIDTH, TEXT_HEIGHT, kColorWhite);

	x += dx;
	y += dy;

	if (x < 0 || x > LCD_COLUMNS - TEXT_WIDTH)
		dx = -dx;

	if (y < 0 || y > LCD_ROWS - TEXT_HEIGHT)
		dy = -dy;

	//pd->graphics->clear(kColorWhite);
	pd->graphics->setFont(font);
	pd->graphics->drawText("Hello World!", strlen("Hello World!"), kASCIIEncoding, x, y);


	pd->system->drawFPS(0, 0);

	return 1;
}
