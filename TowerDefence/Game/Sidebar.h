#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Cursor.h"
#include "Button.h"

using namespace std;

class Sidebar 
{

private:

	void loadButtonsFromFile(string filepath);
public:	
	Sidebar(void);
	Sidebar(Engine &e);	
	Engine engine;

	vector<Button*> buttons;

	Button* findButtonByName(string name);
	void setup(Engine &e);
};
