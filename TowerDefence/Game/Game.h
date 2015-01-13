#pragma once

#include "../Engine/Engine.h"
#include "Constants.h"
#include "Board.h"
#include "Sidebar.h"

using namespace std;

class Game 
{

private:
	
	void setCursorPosition(int x, int y);
	void drawBoardBackground();
	void drawGamePieces();
	void drawBoardForeground();
	void drawCursor();
	void drawDebugFeatures();
	void drawSidebar();

public:	
	Game();	
	~Game();
	Engine engine;
	Board board;
	Sidebar sidebar;
	Cursor cursor;
	char cursorPosition;
	
	void newGame();
	int getInput();
	void update();
	void draw();
	void close();
};
