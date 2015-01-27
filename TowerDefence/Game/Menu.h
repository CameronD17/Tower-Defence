#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Game.h"
#include "ButtonHandler.h"

using namespace std;

class Menu 
{
private:	
	void drawButtons();
	bool gamePlaying;
	ButtonHandler buttonHandler;
	void setHoveredButton(Cursor& cursor);

	int getInput();
	void draw();

	Engine engine;
	Cursor cursor;
	Game game;

public:	
	Menu();	
	~Menu();

	int update();
	void close();
};
