#include "Sidebar.h"

Sidebar::Sidebar(void){}

Sidebar::Sidebar(Engine &e)
{
	engine = e;
}

void Sidebar::setup(Engine &e)	
{
	engine = e;
	loadButtonsFromFile("filepath");
}

Button* Sidebar::findButtonByName(string name)
{
	for (std::vector<Button*>::iterator b = buttons.begin(); b != buttons.end(); ++b)
	{
		if ((*b)->text == name)
		{
			return (*b);
		}
	}
	return NULL;
}

void Sidebar::loadButtonsFromFile(string filepath)
{
	int x, y, w, h;
	string t;
	int id = 1;
	ifstream buttonData("Assets/Inputs/sidebarButtons.txt", ifstream::in);
	if (buttonData.is_open())
	{
		while (buttonData >> x >> y >> w >> h >> t)
		{
			Button * b = new Button(id, x, y, w, h, t);
			buttons.push_back(b);
			id++;
		}
		buttonData.close();
	}
}