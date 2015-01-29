#include "Menu.h"

Menu::Menu()
{
	engine.init();
	cursor.init(engine.resources);
	game.init(engine, cursor);
	buttonHandler.init(engine, "Assets/Inputs/MAIN_MENU_BUTTONS.txt");
	engine.admin.start();
	gamePlaying = false;
}

Menu::~Menu()
{
	close();
}

void Menu::close()
{
	game.close();
	engine.close();
}

// *** DRAW METHODS *** //

void Menu::draw()
{
	engine.graphics.clear();

	drawButtons();

	engine.graphics.update();
}

void Menu::drawButtons()
{
	engine.graphics.renderText((WINDOW_WIDTH / 2 ) - 220, BORDER_SIZE, "Tower Defence", EXTRA_LARGE, 255, 255, 255, "imagine");

	for (vector<Button*>::iterator b = buttonHandler.buttons.begin(); b != buttonHandler.buttons.end(); ++b)
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

void Menu::setHoveredButton(Cursor& cursor)
{
	int i = 0;
	for (vector<Button*>::iterator b = buttonHandler.buttons.begin(); b != buttonHandler.buttons.end(); ++b)
	{
		i++;
		(*b)->cursorOnButton(cursor);
	}
}


// *** UPDATE METHODS *** //

int Menu::update()
{
	int state = UNCHANGED_STATE;

	state = getInput();

	while (gamePlaying)
	{
		state = game.update();
		if (state == EXIT_CURRENT_STATE)
		{
			gamePlaying = false;
		}
		else if (state == EXIT_APPLICATION)
		{
			return EXIT_APPLICATION;
		}
	}

	draw();

	return state;
}

int Menu::getInput()
{
	input k = engine.interfaces.getInput();
	int button = 0;

	if (k.mouseDown)
	{
		for (vector<Button*>::iterator b = buttonHandler.buttons.begin(); b != buttonHandler.buttons.end(); ++b)
		{
			if ((*b)->isHovered())
			{
				button =  (*b)->id;
			}
		}		

		switch (button)
		{
		case 1:		// Campaign PLACEHOLDER
			break;

		case 2:		// Skirmish PLACEHOLDER
			gamePlaying = true;
			break;

		case 3:		// Achievements PLACEHOLDER
			break;

		case 4:		// Highscores PLACEHOLDER
			break;

		case 5:		// Map Editor PLACEHOLDER
			break;

		case 6:		// Settings PLACEHOLDER
			break;

		case 7:		// Exit Application
			return EXIT_APPLICATION;

		default:
			break;
		}
	}
	else if (k.key == SDLK_ESCAPE)
	{
		return EXIT_APPLICATION;
	}
	else
	{
		cursor.setCoordinates(k.x, k.y);
		setHoveredButton(cursor);
	}

	return UNCHANGED_STATE;
}