#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Screen.h"
#include "GameScreen.h"
#include "ButtonHandler.h"

using namespace std;

class MainMenuScreen : public Screen
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
	GameScreen game;

	void closeTransition();
	void openTransition();

public:	
	MainMenuScreen(Engine& e);
	~MainMenuScreen();

	int update();
	void close();
};
