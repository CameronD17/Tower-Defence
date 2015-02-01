#include "Game\MainMenuScreen.h"

int main(int argc, char* args[])
{
	int state = UNCHANGED_STATE;
	Engine engine;
	engine.init();

	MainMenuScreen menu(engine);	

	while (state != EXIT_APPLICATION)
	{
		state = menu.update();
	}
	menu.close();

	return 0;
}