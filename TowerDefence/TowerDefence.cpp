#include "Engine\Engine.h"
#include "Game\Game.h"
#include <stdlib.h>
#include <crtdbg.h>


/*	
 *	There are three main components of this tower defence game - the engine, the menu, and the game itself
 *	The menu gives the player five options - start a new game, resume an existing game, view the highscores, play a minigame (Pong) and quit
 */

int main(int argc, char* args[])
{
	Engine engine;	// Instantiate a new engine
	engine.init();				// Initialise everything engine-related (graphics, resource-management, etc.)
	
	Game g(engine);				// Initialise a new game using the new engine
	g.newGame(0, 0);

	int gameState = 0;		// To check if the player has quit or paused the game
	
	while(gameState != -1)
	{
		gameState = g.run();
	}

	engine.close();					// Finish the game and clean up objects	
	return 0;
}