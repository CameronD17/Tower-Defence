#pragma once
#include "Audio.h"
#include "Graphics.h"
#include "InterfaceManager.h"
#include "Physics.h"
#include "Profiling.h"
#include "ResourceManager.h"
#include "../Game/Constants.h"



/*-------------------------------------------------------- *
 * Engine (Initialise / Shut down) 
 * This class will initialise the game appropriately (start-
 * ing libraries, loading resources etc), and shut down the 
 * game cleanly, cleaning up memory, destroying resources
 * and saving scores where appropriate
 *-------------------------------------------------------- */


class Engine
{
public:	

	Engine();
	~Engine(void);

	// Access all aspects of the engine
	Audio audio;
	Graphics graphics;
	InterfaceManager interfaces;
	Physics physics;
	Profiling admin;
	ResourceManager resources;
	
	// Initialise libraries and load any functions needed
	bool init();

	// This should be the last thing run in the system
	void close(void);
};
