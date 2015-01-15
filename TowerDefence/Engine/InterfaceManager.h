#pragma once
#include <SDL.h>

struct input
{
	bool keyPress;
	SDL_Keycode key;

	bool mouseDown;
	int x, y;
};

class InterfaceManager
{

public:	
	InterfaceManager(void);
	~InterfaceManager(void);

	input getInput();
};
