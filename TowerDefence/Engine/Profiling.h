#pragma once
#include <SDL.h>

/*-------------------------------------------------------- *
 * PROFILING
 * This class will hold all admin functions that are used
 * for testing and debugging purposes, and aren't in-game
 *-------------------------------------------------------- */

class Profiling
{

public:	
	Profiling(void);
	Profiling(int x, int y);
	~Profiling(void);

	int countedFrames;
	float avgFPS;
    unsigned int startTicks;
		
    void start();    
	
	unsigned int getTicks();

	void updateFPS();
	void capFPS();
};
