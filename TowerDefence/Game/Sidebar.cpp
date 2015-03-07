#include "Sidebar.h"

Sidebar::Sidebar(void){}

void Sidebar::setup(Engine &e)	
{
	engine = e;
	buttonHandler.init(engine, "Assets/Inputs/Buttons/IN_GAME_SIDEBAR_BUTTONS.txt");
}

int Sidebar::getInput(input k, Cursor &cursor, Board &board, Notification &notification)
{
	if (k.mouseDown)
	{
		std::string button = buttonHandler.getHoveredButtonName();

		if (button == "Sell")
		{
			board.towerHandler.sell(board.map, board.towerHandler.selectedStats.id, board.bank, notification);
		}
		else if (button == "Upgrade")
		{
			board.towerHandler.upgrade(board.towerHandler.selectedStats.id, board.bank, notification);
		}
		else if (button == "Laser1" || button == "Laser2" || button == "Laser3" || button == "Laser4" || button == "Laser5" 
			|| button == "Laser6" || button == "Laser7" || button == "Laser8" || button == "Laser9" || button == "Laser0")
		{
			board.deselectObject();
			buttonHandler.selectButton(button);
			cursor.setAction(TOWER_1);
		}
		else if (button == "Change Target")
		{
			board.deselectObject();
			buttonHandler.selectButton(button);
			cursor.setAction(CHANGE_TARGET);
		}
		else if (button == "Launch Enemy")
		{
			board.deselectObject();
			buttonHandler.selectButton(button);
			cursor.setAction(LAUNCH_ENEMY);
		}
		else if (button == "Debug")
		{
			if (board.debugMode)
			{
				buttonHandler.deselectButton(button);
				board.debugMode = false;
			}
			else
			{
				buttonHandler.selectButton(button);
				board.debugMode = true;
			}
		}
		else if (button == "Clear")
		{
			board.deselectObject();
			buttonHandler.deselectAllButtons();
			cursor.setAction(CLEAR);
		}
		else if (button == "Pause Game")
		{
			return PAUSED_STATE;
		}
		else if (button == "Exit")
		{
			return EXIT_CURRENT_STATE;
		}
		else if (button == "Auto Launch?")
		{
			board.enemyHandler.autoLaunch(board.map); 
			buttonHandler.selectButton(button);
		}
		else if (button == "Next Wave")
		{
			board.enemyHandler.nextWave();
		}
		else
		{
			board.deselectObject();
			buttonHandler.deselectAllButtons();
			cursor.setAction(0);
		}
	}
	else
	{
		cursor.setCoordinates(k.x, k.y);
		buttonHandler.setHoveredButton(cursor);
	}

	return UNCHANGED_STATE;
}

void Sidebar::update(Board &b)
{
	toggleTowerSelectionButtonVisibility(b.towerHandler.selected);
	toggleAutoLaunchButtons(b.enemyHandler.autolaunch);
}

void Sidebar::draw()
{
	// Divider
	engine.graphics.drawLine(SIDEBAR_X, STATS_Y, WINDOW_WIDTH, STATS_Y);

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

	if (buttonHandler.getButtonSelected())
	{
		std::stringstream buttonText;
		int enemyStatsStartX = SIDEBAR_X + BLOCK_SIZE;
		int enemyStatsStartY = STATS_Y + BLOCK_SIZE;

		buttonText << "Button Selected: " << buttonHandler.getSelectedButton()->text;
		engine.graphics.renderText(enemyStatsStartX, enemyStatsStartY, buttonText.str(), SMALL, 255, 0, 0, "anonymous");
	}
}

void Sidebar::toggleTowerSelectionButtonVisibility(bool towerSelected)
{
	if (towerSelected)
	{
		buttonHandler.findButtonByName("Sell")->show();
		buttonHandler.findButtonByName("Upgrade")->show();
	}
	else
	{
		buttonHandler.findButtonByName("Sell")->hide();
		buttonHandler.findButtonByName("Upgrade")->hide();
	}
}

void Sidebar::toggleAutoLaunchButtons(bool autoLaunch)
{
	if (autoLaunch)
	{
		buttonHandler.findButtonByName("Next Wave")->hide();
	}
	else
	{
		buttonHandler.findButtonByName("Next Wave")->show();
	}
}