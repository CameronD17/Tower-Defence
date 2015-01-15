#include "InterfaceManager.h"

InterfaceManager::InterfaceManager(void)
{
}

InterfaceManager::~InterfaceManager(void)
{
}

input InterfaceManager::getInput()
{
	SDL_Event event;
	input i = {}; //(false, NULL, false, NULL, NULL);	
	int x, y;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			i.keyPress = true;
			i.mouseDown = false;
			i.key = event.key.keysym.sym;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{	
			i.keyPress = false;
			i.mouseDown = true;
		}
	}

	SDL_GetMouseState(&x, &y);
	i.x = x;
	i.y = y;

	return i;
}