#pragma once
#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Screen.h"
#include "Board.h"
#include "Sidebar.h"
#include "PauseMenu.h"
#include "Notification.h"

class GameScreen : public Screen
{
private:	
	Engine engine;
	Cursor cursor;
	Board board;
	Sidebar sidebar;
	PauseMenu pauseMenu;
	Notification notification;

	void closeTransition();
	void openTransition();

public:	
	GameScreen();	
	~GameScreen();

	void init(Engine &e, Cursor &c);
	int getInput();
	int update();
	void draw();

	void loadGame(std::string game);
};
