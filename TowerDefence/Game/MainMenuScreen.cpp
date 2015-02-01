#include "MainMenuScreen.h"

MainMenuScreen::MainMenuScreen(Engine& e)
{
	engine = e;
	cursor.init(engine.resources);
	buttonHandler.init(engine, "Assets/Inputs/Buttons/MAIN_MENU_BUTTONS.txt");
	engine.admin.start();
	transition.init(engine);
	game.init(engine, cursor);
	gamePlaying = false;
	openTransition();
}

MainMenuScreen::~MainMenuScreen()
{
	close();
}

void MainMenuScreen::close()
{
	game.close();
	engine.close();
}

// *** DRAW METHODS *** //

void MainMenuScreen::draw()
{
	engine.graphics.clear();

	drawButtons();
	
	engine.graphics.update();
}

void MainMenuScreen::drawButtons()
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

void MainMenuScreen::setHoveredButton(Cursor& cursor)
{
	int i = 0;
	for (vector<Button*>::iterator b = buttonHandler.buttons.begin(); b != buttonHandler.buttons.end(); ++b)
	{
		i++;
		(*b)->cursorOnButton(cursor);
	}
}


// *** UPDATE METHODS *** //

int MainMenuScreen::update()
{
	int state = UNCHANGED_STATE;

	state = getInput();

	while (gamePlaying)
	{
		state = game.update();
		if (state == EXIT_CURRENT_STATE)
		{
			gamePlaying = false;
			openTransition();
		}
		else if (state == EXIT_APPLICATION)
		{
			return EXIT_APPLICATION;
		}
	}

	draw();

	return state;
}

int MainMenuScreen::getInput()
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
			closeTransition();
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

void MainMenuScreen::closeTransition()
{
	transition.closing = true;
	while (transition.closing)
	{
		transition.close();

		engine.graphics.clear();
		drawButtons();
		transition.draw();
		engine.graphics.update();
	}
}

void MainMenuScreen::openTransition()
{
	transition.opening = true;
	while (transition.opening)
	{
		transition.open();
		
		engine.graphics.clear();
		drawButtons();
		transition.draw();
		engine.graphics.update();
	}
}