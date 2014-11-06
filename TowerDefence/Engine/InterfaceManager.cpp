#include "InterfaceManager.h"

InterfaceManager::InterfaceManager(void)
{
}

InterfaceManager::~InterfaceManager(void)
{
}

input InterfaceManager::getInput()
{
	// Add a new event to handle anything the user does
	SDL_Event event;
	input thisInput = {};//(false, NULL, false, NULL, NULL);	
	int x, y;

	// Return a keypress
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			thisInput.keyPress = true;
			thisInput.key = event.key.keysym.sym;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_GetMouseState(&x, &y);

			thisInput.keyPress = false;
			thisInput.mouseDown = true;
			thisInput.x = x;
			thisInput.y = y;
		}
		else if (event.type == SDL_MOUSEMOTION)
		{
			thisInput.mouseDown = false;
			SDL_GetMouseState(&x, &y);
			if (x > 0 && y > 0)
			{
				thisInput.x = x;
				thisInput.y = y;
			}
		}
	}

	return thisInput;
}




/*
SDL_Keycode InterfaceManager::getInput()
{
	// Add a new event to handle anything the user does
	SDL_Event event;

	// Return a keypress
	while(SDL_PollEvent(&event)) 
	{
		if (event.type == SDL_KEYDOWN) 
		{
			return event.key.keysym.sym;
		}		
	}

	return NULL;
}*/

SDL_Event InterfaceManager::getGeneralInput()
{
	SDL_Event event;
	
	while(SDL_PollEvent(&event)) 
	{
		return event;
	}

	return event;
}