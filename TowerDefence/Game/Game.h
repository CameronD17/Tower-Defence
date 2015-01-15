#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Board.h"
#include "Sidebar.h"

using namespace std;

class Game 
{

private:
	
	void drawBoardBackground();
	void drawGamePieces();
	void drawBoardForeground();
	void drawCursor();
	void drawDebugFeatures();
	void drawSidebar();
	void drawSidebarEnemyStats();
	void drawSidebarTowerStats();
	void drawButtons();

public:	
	Game();	
	~Game();
	Engine engine;
	Board board;
	Sidebar sidebar;
	Cursor cursor;

	int getInput();
	int handleBoardInput(input k);
	int handleSidebarInput(input k);
	void update();
	void draw();
	void close();
};
