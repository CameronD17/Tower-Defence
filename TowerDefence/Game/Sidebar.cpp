#include "Sidebar.h"

Sidebar::Sidebar(void){}

Sidebar::Sidebar(Engine &e)
{
	engine = e;
}

void Sidebar::setup(Engine &e)	
{
	engine = e;
	buttonHandler.init(engine, "Assets/Inputs/Buttons/IN_GAME_SIDEBAR_BUTTONS.txt");
}

void Sidebar::update(bool tower)
{
	toggleTowerSelectionButtonVisibility(tower);
}

void Sidebar::toggleTowerSelectionButtonVisibility(bool towerSelected)
{
	if (towerSelected)
	{
		buttonHandler.findButtonByName("Sell")->show();
	}
	else
	{
		buttonHandler.findButtonByName("Sell")->hide();
	}
}