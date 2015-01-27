#pragma once
#include "Audio.h"
#include "Graphics.h"
#include "InterfaceManager.h"
#include "Physics.h"
#include "Profiling.h"
#include "ResourceManager.h"
#include "../Engine/Constants.h"

class Engine
{
public:	

	Engine();
	~Engine(void);

	bool init();

	Audio audio;
	Graphics graphics;
	InterfaceManager interfaces;
	Physics physics;
	Profiling admin;
	ResourceManager resources;

	void close(void);
};
