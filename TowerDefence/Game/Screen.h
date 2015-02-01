#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "ScreenTransition.h"

using namespace std;

class Screen
{
private:	
	Engine engine;

public:	
	Screen();
	Screen(Engine& e);
	~Screen();

	ScreenTransition transition;

	int update();
};
