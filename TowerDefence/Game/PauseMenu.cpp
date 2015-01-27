#include "PauseMenu.h"

PauseMenu::PauseMenu()
{
	
}

PauseMenu::~PauseMenu()
{
	close();
}

void PauseMenu::setup(Engine &e, Cursor &c)
{
	engine = e;
	cursor = c;
	paused = false;
	buttonHandler.init(engine, "Assets/Inputs/PAUSE_MENU_BUTTONS.txt");
}

void PauseMenu::close()
{

}

// *** UPDATE METHODS *** //

int PauseMenu::update()
{	
	int returning = getInput();

	return returning;
}

int PauseMenu::getInput()
{
	input k = engine.interfaces.getInput();

	if (k.keyPress)
	{
		switch (k.key)
		{
		case SDLK_ESCAPE:
			return EXIT_CURRENT_STATE;

		case SDLK_q:
			return EXIT_APPLICATION;

		case SDLK_p:
			pause();

		default:
			break;
		}
	}	
	else if (k.mouseDown)
	{
		int button = buttonHandler.getHoveredButtonId();

		switch (button)
		{
		case 1:		
			pause();
			break;

		case 2: 
			pause();
			return EXIT_CURRENT_STATE;

		case 3:
			pause();
			return EXIT_APPLICATION;

		default:
			break;
		}
	}
	else
	{
		cursor.setCoordinates(k.x, k.y);
		buttonHandler.setHoveredButton(cursor);
	}

	return UNCHANGED_STATE;
}

void PauseMenu::pause()
{
	if (paused)
	{
		paused = false;
	}
	else
	{
		paused = true;
	}
}