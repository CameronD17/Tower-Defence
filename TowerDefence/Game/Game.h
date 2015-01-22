#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Board.h"
#include "Sidebar.h"

using namespace std;

class Game 
{
private:	
	void drawBoardMap();
	void drawBoardPieces();
	void drawCursor();
	void drawDebugFeatures();

	void drawSidebar();
	void drawSidebarEnemyStats();
	void drawSidebarTowerStats();
	void drawSidebarSelectedButton();
	void drawSidebarButtons();

public:	
	Game();	
	~Game();
	Engine engine;
	Cursor cursor;
	Board board;
	Sidebar sidebar;

	int getInput();
	int handleBoardInput(input k);
	int handleSidebarInput(input k);
	void update();
	void draw();
	void close();
};
