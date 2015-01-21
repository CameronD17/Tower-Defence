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
	ifstream buttonData("Assets/Inputs/IN_GAME_SIDEBAR_BUTTONS.txt");
	string buttonName;

	while (!std::getline(buttonData, buttonName, '~').eof())
	{
		string id, x, y, width, height, fontSize, offset, buttonText, visible;

		getline(buttonData, id, '~');
		getline(buttonData, x, '~');
		getline(buttonData, y, '~');
		getline(buttonData, width, '~');
		getline(buttonData, height, '~');
		getline(buttonData, fontSize, '~');
		getline(buttonData, offset, '~');
		getline(buttonData, buttonText, '~');
		getline(buttonData, visible);

		Button * b = new Button(stoi(id), stoi(x), stoi(y), stoi(width), stoi(height), stoi(fontSize), stoi(offset), buttonText, stoi(visible) == 1);
		buttons.push_back(b);
	}

	buttonData.close();
}