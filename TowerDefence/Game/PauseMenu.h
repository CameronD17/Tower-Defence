#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Cursor.h"
#include "ButtonHandler.h"

using namespace std;

class PauseMenu 
{
public:	
	PauseMenu();	
	~PauseMenu();
	Engine engine;
	Cursor cursor;
	
	ButtonHandler buttonHandler;
	vector<Button*> buttons;

	bool paused;
	void pause();

	int getInput();

	int update();

	void setup(Engine &e, Cursor &c);
	void close();
};
