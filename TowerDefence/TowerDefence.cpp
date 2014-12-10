#include "Engine\Engine.h"
#include "Game\Game.h"
#include <stdlib.h>
#include <crtdbg.h>

// TESTING PUSH FOR SAM 10-DEC 21:45
// COLLABORATION A-GO-GO!!

#define UPDATESPERSECOND 60
#define SKIPTICKS (1000 / UPDATESPERSECOND)

int main(int argc, char* args[])
{
	Engine engine;
	engine.init();

	Game game(engine);	
	game.newGame();

	int gameState = 0;

	float nextUpdate = SDL_GetTicks();
	unsigned int currentTime;
	
	while (gameState != -1) 
	{
		currentTime = SDL_GetTicks();

		gameState = game.getInput();

		while (nextUpdate < currentTime) 
		{
			game.update();
			nextUpdate += SKIPTICKS;
		}

		float interpolation = (currentTime + SKIPTICKS - nextUpdate) / SKIPTICKS;
		game.draw(interpolation);
	}


	engine.close();
	return 0;
}