#include "InterfaceManager.h"

InterfaceManager::InterfaceManager(void)
{
}

InterfaceManager::~InterfaceManager(void)
{
}

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
}

SDL_Event InterfaceManager::getGeneralInput()
{
	SDL_Event event;
	
	while(SDL_PollEvent(&event)) 
	{
		return event;
	}
}