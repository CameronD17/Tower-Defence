#include "Profiling.h"
#include "../Game/Constants.h"

Profiling::Profiling(void)
{
}

Profiling::~Profiling(void)
{
}

void Profiling::start()
{
    startTicks = SDL_GetTicks();
	countedFrames = 0;
	avgFPS = 0;
}

unsigned int Profiling::getTicks()
{
	unsigned int time = SDL_GetTicks() - startTicks;
    return time;
}

void Profiling::updateFPS()
{
	avgFPS = countedFrames / (getTicks() / 1000.f);
	if(avgFPS > 2000000) avgFPS = 0;
}

// Not actually used in game - delete?
void Profiling::capFPS()
{
	int framerate = getTicks();
	if(framerate < SCREEN_TICK)
	{
		SDL_Delay(SCREEN_TICK - framerate);
	}
}