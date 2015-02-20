#pragma once
#include <SDL.h>

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
