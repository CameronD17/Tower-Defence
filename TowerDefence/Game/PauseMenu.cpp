#include "PauseMenu.h"

PauseMenu::PauseMenu()
{
	
}

PauseMenu::~PauseMenu()
{
}

void PauseMenu::setup(Engine &e, Cursor &c)
{
	engine = e;
	cursor = c;
	paused = false;
	buttonHandler.init(engine, "Assets/Inputs/Buttons/IN_GAME_PAUSE_MENU_BUTTONS.txt");
}

void PauseMenu::draw()
{
	if (paused)
	{
		engine.graphics.drawRectangle(PAUSE_MENU_X, PAUSE_MENU_Y, PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT, 0, 0, 0);
		engine.graphics.drawRectangleOL(PAUSE_MENU_X - 1, PAUSE_MENU_Y - 1, PAUSE_MENU_WIDTH + 2, PAUSE_MENU_HEIGHT + 2, 255, 255, 255);
		engine.graphics.renderText(PAUSE_MENU_X + 150, PAUSE_MENU_Y + 30, "Pause", LARGE, 255, 255, 255, "bombardier");

		for (std::vector<Button*>::iterator b = buttonHandler.buttons.begin(); b != buttonHandler.buttons.end(); ++b)
		{
			if ((*b)->isVisible())
			{
				if ((*b)->isHovered())
				{
					engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 0, 255);
					engine.graphics.renderText((*b)->getX() + (*b)->getOffset(), (*b)->getY() + (*b)->getOffset(), (*b)->text, (*b)->getFontSize(), 255, 0, 255, "anonymous");
				}
				else if ((*b)->isSelected())
				{
					engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 255, 0);
					engine.graphics.renderText((*b)->getX() + (*b)->getOffset(), (*b)->getY() + (*b)->getOffset(), (*b)->text, (*b)->getFontSize(), 255, 255, 0, "anonymous");
				}
				else
				{
					engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 255, 255, 255);
					engine.graphics.renderText((*b)->getX() + (*b)->getOffset(), (*b)->getY() + (*b)->getOffset(), (*b)->text, (*b)->getFontSize(), 255, 255, 255, "anonymous");
				}
			}
		}
	}
}

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