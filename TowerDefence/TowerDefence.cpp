#include "Game\Menu.h"

int main(int argc, char* args[])
{
	int state = UNCHANGED_STATE;

	Menu menu;
	while (state != EXIT_APPLICATION)
	{
		state = menu.update();
	}
	menu.close();

	return 0;
}