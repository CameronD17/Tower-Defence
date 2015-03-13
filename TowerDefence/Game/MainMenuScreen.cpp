#include "MainMenuScreen.h"

MainMenuScreen::MainMenuScreen(Engine& e)
{
	engine = e;
	engine.admin.start();
	cursor.init(engine);
	buttonHandler.init(engine, "Assets/Inputs/Buttons/MAIN_MENU_BUTTONS.txt");
	transition.init(engine);
	game.init(engine, cursor);
	skirmish.setup(engine, cursor);
	gamePlaying = false;
	focused = true;
	openTransition();
}

MainMenuScreen::~MainMenuScreen(){}

int MainMenuScreen::getInput()
{
	input k = engine.interfaces.getInput();
	int button = 0;

	if (k.mouseDown)
	{
		for (std::vector<Button*>::iterator b = buttonHandler.buttons.begin(); b != buttonHandler.buttons.end(); ++b)
		{
			if ((*b)->isHovered())
			{
				button = (*b)->id;
			}
		}

		switch (button)
		{
		case 1:		// Campaign PLACEHOLDER
			break;

		case 2:		// Skirmish PLACEHOLDER
			skirmish.selected = skirmish.selected ? false : true;
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
	else if (k.key == SDLK_ESCAPE || k.key == SDLK_q)
	{
		return EXIT_APPLICATION;
	}
	else if (k.key == SDLK_n)
	{
		gamePlaying = true;
		game.loadGame("Assets/Save_Games/Skirmish/Map_1/");
		closeTransition();
	}
	else
	{
		cursor.setCoordinates(k.x, k.y);
		buttonHandler.setHoveredButton(cursor);
	}

	return UNCHANGED_STATE;
}

void MainMenuScreen::draw()
{
	engine.graphics.clear();

	drawButtons();

	skirmish.draw();

	engine.graphics.update();
}

void MainMenuScreen::drawButtons()
{
	engine.graphics.renderText((WINDOW_WIDTH / 2) - 220, BORDER_SIZE, "Tower Defence", EXTRA_LARGE, 255, 255, 255, "imagine");

	if (focused)
	{
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
	else
	{
		for (std::vector<Button*>::iterator b = buttonHandler.buttons.begin(); b != buttonHandler.buttons.end(); ++b)
		{
			if ((*b)->isVisible())
			{
				engine.graphics.drawRectangleOL((*b)->getX(), (*b)->getY(), (*b)->getW(), (*b)->getH(), 96, 96, 96);
				engine.graphics.renderText((*b)->getX() + (*b)->getOffset(), (*b)->getY() + (*b)->getOffset(), (*b)->text, (*b)->getFontSize(), 96, 96, 96, "anonymous");
			}
		}
	}
}

int MainMenuScreen::update()
{
	focused = true;
	int state = getInput();

	while (skirmish.selected)
	{	
		focused = false;
		draw();
		state = skirmish.getInput();
		if (state == EXIT_CURRENT_STATE)
		{
			skirmish.resetValues();
		}
		else if (state == EXIT_APPLICATION)
		{
			closeTransition();
			return EXIT_APPLICATION;
		}
		else if (state > 1)
		{
			gamePlaying = true;
			game.loadGame(skirmish.getSelectedButtonString());
			closeTransition();
			skirmish.resetValues();
		}
	}

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