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

	int getInput();
	void draw();

public:	
	GameScreen();	
	~GameScreen();

	void init(Engine &e, Cursor &c);
	void loadGame(std::string game);

	int update();
};
