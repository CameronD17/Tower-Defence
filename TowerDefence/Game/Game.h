#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Board.h"
#include "Sidebar.h"
#include "PauseMenu.h"

using namespace std;

class Game 
{
private:	
	void drawBoardMap();
	void drawBoardPieces();
	void drawCursor();
	void drawPauseMenu();
	void drawDebugFeatures();

	void drawSidebar();
	void drawSidebarEnemyStats();
	void drawSidebarTowerStats();
	void drawSidebarSelectedButton();
	void drawSidebarButtons();

	Engine engine;
	Cursor cursor;
	Board board;
	Sidebar sidebar;
	PauseMenu pauseMenu;

public:	
	Game();	
	~Game();

	int getInput();
	int handleBoardInput(input k);
	int handleSidebarInput(input k);

	int update();
	void draw();

	void init(Engine &e, Cursor &c);
	void close();
};
