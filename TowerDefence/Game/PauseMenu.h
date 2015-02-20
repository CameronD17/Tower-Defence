#pragma once
#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Cursor.h"
#include "ButtonHandler.h"

class PauseMenu 
{
private:
	Engine engine;
	Cursor cursor;
	ButtonHandler buttonHandler;
	std::vector<Button*> buttons;

public:	
	PauseMenu();	
	~PauseMenu();

	bool paused;
	void pause();

	int getInput();
	int update();
	void draw();

	void setup(Engine &e, Cursor &c);
};
