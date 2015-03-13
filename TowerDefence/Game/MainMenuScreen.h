#pragma once
#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Screen.h"
#include "MainMenuSkirmishSubMenu.h"
#include "GameScreen.h"
#include "ButtonHandler.h"

class MainMenuScreen : public Screen
{
private:	
	bool gamePlaying;
	bool focused;

	int getInput();
	void draw();
	void drawButtons();

	Engine engine;
	Cursor cursor;
	GameScreen game;
	ButtonHandler buttonHandler;

	MainMenuSkirmishSubMenu skirmish;

	void closeTransition();
	void openTransition();

public:	
	MainMenuScreen(Engine& e);
	~MainMenuScreen();

	int update();
};
