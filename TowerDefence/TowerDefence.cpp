#include "Engine\Engine.h"
#include "Game\Game.h"
#include <stdlib.h>
#include <crtdbg.h>

#define UPDATESPERSECOND 10
#define SKIPTICKS (1000 / UPDATESPERSECOND)

int main(int argc, char* args[])
{
	Engine engine;
	engine.init();

	Game game(engine);	
	game.newGame();

	int gameState = 0;

	float nextUpdate = SDL_GetTicks();
	float currentTime;
	
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