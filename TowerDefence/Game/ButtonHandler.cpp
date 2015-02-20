#include "ButtonHandler.h"

ButtonHandler::ButtonHandler()
{

}

void ButtonHandler::init(Engine& e, std::string filepath)
{
	engine = e;
	loadButtonsFromFile(filepath);
}

Button* ButtonHandler::findButtonByName(std::string name)
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

Button* ButtonHandler::findButtonById(int i)
{
	for (std::vector<Button*>::iterator b = buttons.begin(); b != buttons.end(); ++b)
	{
		if ((*b)->id == i)
		{
			return (*b);
		}
	}
	return NULL;
}

Button* ButtonHandler::getSelectedButton()
{
	return selectedButton;
}

bool ButtonHandler::getButtonSelected()
{
	return buttonSelected;
}

int ButtonHandler::getHoveredButtonId()
{
	for (std::vector<Button*>::iterator b = buttons.begin(); b != buttons.end(); ++b)
	{
		if ((*b)->isHovered())
		{
			return (*b)->id;
		}
	}

	return 0;
}

std::string ButtonHandler::getHoveredButtonName()
{
	for (std::vector<Button*>::iterator b = buttons.begin(); b != buttons.end(); ++b)
	{
		if ((*b)->isHovered())
		{
			return (*b)->text;
		}
	}

	return "";
}

int ButtonHandler::getSelectedButtonId()
{
	for (std::vector<Button*>::iterator b = buttons.begin(); b != buttons.end(); ++b)
	{
		if ((*b)->isSelected())
		{
			return (*b)->id;
		}
	}

	return 0;
}

void ButtonHandler::setHoveredButton(Cursor& cursor)
{
	int i = 0;
	for (std::vector<Button*>::iterator b = buttons.begin(); b != buttons.end(); ++b)
	{
		i++;
		(*b)->cursorOnButton(cursor);
	}
}

void ButtonHandler::selectButton(int i)
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

void ButtonHandler::selectButton(std::string s)
{
	buttonSelected = true;

	for (std::vector<Button*>::iterator b = buttons.begin(); b != buttons.end(); ++b)
	{
		if ((*b)->text == s)
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

void ButtonHandler::deselectButton(int i)
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

void ButtonHandler::deselectButton(std::string s)
{
	buttonSelected = false;

	for (std::vector<Button*>::iterator b = buttons.begin(); b != buttons.end(); ++b)
	{
		if ((*b)->text == s)
		{
			(*b)->deselect();
		}
	}
}

void ButtonHandler::deselectAllButtons()
{
	buttonSelected = false;

	for (std::vector<Button*>::iterator b = buttons.begin(); b != buttons.end(); ++b)
	{
		(*b)->deselect();
	}
}

void ButtonHandler::loadButtonsFromFile(std::string filepath)
{
	std::ifstream buttonData(filepath.c_str());
	std::string buttonName;

	while (!getline(buttonData, buttonName, '~').eof())
	{
		std::string id, x, y, width, height, fontSize, offset, buttonText, visible;

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