#include "Game\Game.h"

int main(int argc, char* args[])
{
	Game game;	

	int state = 0;

	while (state != -1) 
	{
		state = game.getInput();
		game.update();
		game.draw();
	}

	game.close();
	return 0;
}