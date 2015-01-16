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

void Sidebar::selectButton(int i)
{
	buttonSelected = true;

	for (std::vector<Button*>::iterator b = buttons.begin(); b != buttons.end(); ++b)
	{
		if ((*b)->id == i)
		{
			(*b)->select();
			selectedButton = (*b);
		}
		else
		{
			(*b)->deselect();
		}
	}
}

void Sidebar::deselectButton(int i)
{
	buttonSelected = false;

	for (std::vector<Button*>::iterator b = buttons.begin(); b != buttons.end(); ++b)
	{
		if ((*b)->id == i)
		{
			(*b)->deselect();
		}
	}
}

void Sidebar::deselectAllButtons()
{
	buttonSelected = false;

	for (std::vector<Button*>::iterator b = buttons.begin(); b != buttons.end(); ++b)
	{
		(*b)->deselect();
	}
}

void Sidebar::loadButtonsFromFile(string filepath)
{
	int x, y, w, h, s, o;
	string t; bool v;
	int id = 1;
	ifstream buttonData("Assets/Inputs/sidebarButtons.txt", ifstream::in);
	if (buttonData.is_open())
	{
		while (buttonData >> x >> y >> w >> h >> s >> o >> t >> v)
		{
			if (t == "NULL")
			{
				Button * b = new Button(id, x, y, w, h, s, o, "", v);
				buttons.push_back(b);
			}
			else
			{
				Button * b = new Button(id, x, y, w, h, s, o, t, v);
				buttons.push_back(b);
			}
			id++;
		}
		buttonData.close();
	}
}