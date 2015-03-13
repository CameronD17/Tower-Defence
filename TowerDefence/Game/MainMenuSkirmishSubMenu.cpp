#include "MainMenuSkirmishSubMenu.h"

MainMenuSkirmishSubMenu::MainMenuSkirmishSubMenu(){}

MainMenuSkirmishSubMenu::~MainMenuSkirmishSubMenu(){}

void MainMenuSkirmishSubMenu::setup(Engine &e, Cursor &c)
{
	engine = e;
	cursor = c;
	buttonHandler.init(engine, "Assets/Inputs/Buttons/MAIN_MENU_SKIRMISH_BUTTONS.txt");
	resetValues();
}

int MainMenuSkirmishSubMenu::getInput()
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

		default:
			break;
		}
	}
	else if (k.mouseDown)
	{
		int button = buttonHandler.getHoveredButtonId();

		switch (button)
		{
		case EXIT_SUBMENU:
			return EXIT_CURRENT_STATE;

		case MAP_1: case MAP_2: case MAP_3: case MAP_4: case MAP_5:
			whichMap = (button / SAVE_GAME_PANEL_BUTTONS) + 1;
			buttonHandler.selectButton(button);
			handleMapButtonClick();
			break;

		default:
			buttonHandler.selectButton(button);
			return button;
		}
	}
	else
	{
		cursor.setCoordinates(k.x, k.y);
		buttonHandler.setHoveredButton(cursor);
		handleMapButtonHover();
	}

	return UNCHANGED_STATE;
}

void MainMenuSkirmishSubMenu::draw()
{
	if (selected)
	{
		// Submenu background, submenu outline, submenu heading, submenu map panel outline
		engine.graphics.drawRectangle(LOAD_GAME_SUBMENU_X, LOAD_GAME_SUBMENU_Y, LOAD_GAME_SUBMENU_WIDTH, LOAD_GAME_SUBMENU_HEIGHT, 0, 0, 0);
		engine.graphics.drawRectangleOL(LOAD_GAME_SUBMENU_X - 1, LOAD_GAME_SUBMENU_Y - 1, LOAD_GAME_SUBMENU_WIDTH + 2, LOAD_GAME_SUBMENU_HEIGHT + 2, 255, 255, 255);
		engine.graphics.renderText(LOAD_GAME_SUBMENU_X + 150, LOAD_GAME_SUBMENU_Y + 30, "Skirmish", LARGE, 255, 255, 255, "bombardier");
		engine.graphics.drawRectangleOL(LOAD_GAME_SUBMENU_INTERNAL_X - 1, LOAD_GAME_SUBMENU_INTERNAL_Y - 1, LOAD_GAME_SUBMENU_INTERNAL_WIDTH + 2, LOAD_GAME_SUBMENU_INTERNAL_HEIGHT + 2, 255, 255, 255);

		if (buttonHandler.getHoveredButtonId() % SAVE_GAME_PANEL_BUTTONS == 2)
		{
			switch (buttonHandler.getHoveredButtonId() / SAVE_GAME_PANEL_BUTTONS)
			{
			case 0:
				engine.graphics.drawRectangle(LOAD_GAME_SUBMENU_INTERNAL_X, LOAD_GAME_SUBMENU_INTERNAL_Y, LOAD_GAME_SUBMENU_INTERNAL_WIDTH, LOAD_GAME_SUBMENU_INTERNAL_HEIGHT, 0, 0, 255);
				break;
			case 1:
				engine.graphics.drawRectangle(LOAD_GAME_SUBMENU_INTERNAL_X, LOAD_GAME_SUBMENU_INTERNAL_Y, LOAD_GAME_SUBMENU_INTERNAL_WIDTH, LOAD_GAME_SUBMENU_INTERNAL_HEIGHT, 255, 0, 0);
				break;
			case 2:
				engine.graphics.drawRectangle(LOAD_GAME_SUBMENU_INTERNAL_X, LOAD_GAME_SUBMENU_INTERNAL_Y, LOAD_GAME_SUBMENU_INTERNAL_WIDTH, LOAD_GAME_SUBMENU_INTERNAL_HEIGHT, 0, 255, 0);
				break;
			case 3:
				engine.graphics.drawRectangle(LOAD_GAME_SUBMENU_INTERNAL_X, LOAD_GAME_SUBMENU_INTERNAL_Y, LOAD_GAME_SUBMENU_INTERNAL_WIDTH, LOAD_GAME_SUBMENU_INTERNAL_HEIGHT, 255, 0, 255);
				break;
			case 4:
				engine.graphics.drawRectangle(LOAD_GAME_SUBMENU_INTERNAL_X, LOAD_GAME_SUBMENU_INTERNAL_Y, LOAD_GAME_SUBMENU_INTERNAL_WIDTH, LOAD_GAME_SUBMENU_INTERNAL_HEIGHT, 0, 255, 255);
				break;
			default:
				break;
			}
		}
		else
		{
			switch (whichMap)
			{
			case 1:
				engine.graphics.drawRectangle(LOAD_GAME_SUBMENU_INTERNAL_X, LOAD_GAME_SUBMENU_INTERNAL_Y, LOAD_GAME_SUBMENU_INTERNAL_WIDTH, LOAD_GAME_SUBMENU_INTERNAL_HEIGHT, 0, 0, 255);
				break;
			case 2:
				engine.graphics.drawRectangle(LOAD_GAME_SUBMENU_INTERNAL_X, LOAD_GAME_SUBMENU_INTERNAL_Y, LOAD_GAME_SUBMENU_INTERNAL_WIDTH, LOAD_GAME_SUBMENU_INTERNAL_HEIGHT, 255, 0, 0);
				break;
			case 3:
				engine.graphics.drawRectangle(LOAD_GAME_SUBMENU_INTERNAL_X, LOAD_GAME_SUBMENU_INTERNAL_Y, LOAD_GAME_SUBMENU_INTERNAL_WIDTH, LOAD_GAME_SUBMENU_INTERNAL_HEIGHT, 0, 255, 0);
				break;
			case 4:
				engine.graphics.drawRectangle(LOAD_GAME_SUBMENU_INTERNAL_X, LOAD_GAME_SUBMENU_INTERNAL_Y, LOAD_GAME_SUBMENU_INTERNAL_WIDTH, LOAD_GAME_SUBMENU_INTERNAL_HEIGHT, 255, 0, 255);
				break;
			case 5:
				engine.graphics.drawRectangle(LOAD_GAME_SUBMENU_INTERNAL_X, LOAD_GAME_SUBMENU_INTERNAL_Y, LOAD_GAME_SUBMENU_INTERNAL_WIDTH, LOAD_GAME_SUBMENU_INTERNAL_HEIGHT, 0, 255, 255);
				break;
			default:
				break;
			}
		}

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

void MainMenuSkirmishSubMenu::handleMapButtonClick()
{
	int selected = buttonHandler.getSelectedButtonId();
	for (std::vector<Button*>::iterator b = buttonHandler.buttons.begin(); b != buttonHandler.buttons.end(); ++b)
	{
		if ((*b)->id % SAVE_GAME_PANEL_BUTTONS != 2 && (*b)->id > 1)
		{
			(*b)->hide();
		}
	}

	for (int j = selected; j <= selected + SAVE_GAME_PANEL_BUTTONS - 2; ++j)
	{
		buttonHandler.buttons.at(j)->show();
	}
}

void MainMenuSkirmishSubMenu::handleMapButtonHover()
{
	int hovered = buttonHandler.getHoveredButtonId() / SAVE_GAME_PANEL_BUTTONS;

	if (whichMap != hovered + 1 && buttonHandler.getHoveredButtonId() % SAVE_GAME_PANEL_BUTTONS == 2)
	{
		for (std::vector<Button*>::iterator b = buttonHandler.buttons.begin(); b != buttonHandler.buttons.end(); ++b)
		{
			if ((*b)->id % SAVE_GAME_PANEL_BUTTONS != 2 && (*b)->id > 1)
			{
				(*b)->hide();
			}
		}
	}
	else if (whichMap != 0)
	{
		for (int j = buttonHandler.buttons.at(whichMap)->id; j < buttonHandler.buttons.at(whichMap + SAVE_GAME_PANEL_BUTTONS)->id; ++j)
		{
			buttonHandler.buttons.at(j)->show();
		}
	}
}

std::string MainMenuSkirmishSubMenu::getSelectedButtonString()
{
	std::stringstream gameSaveFilePathString;
	gameSaveFilePathString << "Assets/Save_Games/Skirmish/";

	switch (whichMap)
	{
	case 1:
		gameSaveFilePathString << "Map_1/";
		break;
	case 2:
		gameSaveFilePathString << "Map_2/";
		break;
	case 3:
		gameSaveFilePathString << "Map_3/";
		break;
	case 4:
		gameSaveFilePathString << "Map_4/";
		break;
	case 5:
		gameSaveFilePathString << "Map_5/";
		break;
	default:
		break;
	}

	int id = buttonHandler.getHoveredButtonId();

	switch (id)
	{
	case MAP_1_SAVE_1: case MAP_2_SAVE_1: case MAP_3_SAVE_1: case MAP_4_SAVE_1: case MAP_5_SAVE_1:
		gameSaveFilePathString << "Save_1/";
		break;
	case MAP_1_SAVE_2: case MAP_2_SAVE_2: case MAP_3_SAVE_2: case MAP_4_SAVE_2: case MAP_5_SAVE_2:
		gameSaveFilePathString << "Save_2/";
		break;
	case MAP_1_SAVE_3: case MAP_2_SAVE_3: case MAP_3_SAVE_3: case MAP_4_SAVE_3: case MAP_5_SAVE_3:
		gameSaveFilePathString << "Save_3/";
		break;
	case MAP_1_SAVE_4: case MAP_2_SAVE_4: case MAP_3_SAVE_4: case MAP_4_SAVE_4: case MAP_5_SAVE_4:
		gameSaveFilePathString << "Save_4/";
		break;
	default:
		break;
	}

	return gameSaveFilePathString.str();
}

void MainMenuSkirmishSubMenu::resetValues()
{
	whichMap = 0;
	selected = false;

	for (std::vector<Button*>::iterator b = buttonHandler.buttons.begin(); b != buttonHandler.buttons.end(); ++b)
	{
		if ((*b)->id % SAVE_GAME_PANEL_BUTTONS != 2 && (*b)->id > 1)
		{
			(*b)->hide();
		}
	}
}