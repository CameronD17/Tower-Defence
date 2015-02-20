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
	ButtonHandler buttonHandler;

	int getInput();
	void draw();
	void drawButtons();

	bool focused;

	Engine engine;
	Cursor cursor;
	GameScreen game;

	MainMenuSkirmishSubMenu skirmish;

	void closeTransition();
	void openTransition();

public:	
	MainMenuScreen(Engine& e);
	~MainMenuScreen();

	int update();
};
