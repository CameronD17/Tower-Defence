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

public:	
	PauseMenu();	
	~PauseMenu();
	
	void setup(Engine &e, Cursor &c);

	void pause();
	bool paused;

	int getInput();
	void draw();
};
