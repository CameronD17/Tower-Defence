#include "Sidebar.h"

Sidebar::Sidebar(void){}

Sidebar::Sidebar(Engine &e)
{
	engine = e;
}

void Sidebar::setup(Engine &e)	
{
	engine = e;
	buttonHandler.init(engine, "filepath");
}

void Sidebar::update(bool tower)
{
	toggleTowerSelectionButtonVisibility(tower);
}

int Sidebar::getHoveredButtonId()
{
	for (std::vector<Button*>::iterator b = buttonHandler.buttons.begin(); b != buttonHandler.buttons.end(); ++b)
	{
		if ((*b)->isHovered())
		{
			return (*b)->id;
		}
	}

	return 0;
}

void Sidebar::setHoveredButton(Cursor& cursor)
{
	int i = 0;
	for (std::vector<Button*>::iterator b = buttonHandler.buttons.begin(); b != buttonHandler.buttons.end(); ++b)
	{
		i++;
		(*b)->cursorOnButton(cursor);
	}
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