#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Cursor.h"
#include "ButtonHandler.h"

class MainMenuSkirmishSubMenu 
{
private:
	Engine engine;
	Cursor cursor;
	ButtonHandler buttonHandler;

	int getInput();
	void handleMapButtonClick();
	void handleMapButtonHover();

public:	
	MainMenuSkirmishSubMenu();	
	~MainMenuSkirmishSubMenu();
	void setup(Engine &e, Cursor &c);

	int whichMap;

	void select();
	bool selected;

	int update();
	void draw();

	void resetValues();
	std::string getSelectedButtonString();
};
