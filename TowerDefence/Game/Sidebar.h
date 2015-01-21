#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Cursor.h"
#include "Button.h"
#include <sstream>

using namespace std;

class Sidebar 
{

private:
	void loadButtonsFromFile(string filepath);

public:	
	Sidebar(void);
	Sidebar(Engine &e);	

	bool buttonSelected;

	Engine engine;

	Button* selectedButton;
	vector<Button*> buttons;

	void selectButton(int id);
	void deselectButton(int id);
	void deselectAllButtons();

	Button* findButtonByName(string name);
	void setup(Engine &e);
};
