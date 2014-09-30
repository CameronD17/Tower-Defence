#pragma once
#include <SDL.h>


/*-------------------------------------------------------- *
 * INTERFACE MANAGER
 * This class will handle all inputs by the player, and send
 * results to be dealt with elsewhere in the engine
 *-------------------------------------------------------- */



class InterfaceManager
{

public:	
	InterfaceManager(void);
	InterfaceManager(int x, int y);
	~InterfaceManager(void);

	SDL_Keycode getInput();
	SDL_Event getGeneralInput();
};
