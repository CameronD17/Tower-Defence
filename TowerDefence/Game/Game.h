#pragma once

#include "../Engine/Engine.h"
#include "Constants.h"
#include "Board.h"
#include "Sidebar.h"

using namespace std;

class Game 
{

private:
	// *** VARIABLES *** //

	
	// *** METHODS *** //
			

	// Draw methods
	void drawBoardBackground();
	void drawGamePieces();
	void drawBoardForeground();
	void drawCursor();
	void drawDebugFeatures();


public:	
	Game(void);
	Game(Engine &sc);	
	Engine engine;
	Board board;
	Sidebar sidebar;
	
	void newGame();
	int getInput();
	void update();
	void draw(float interpolation);
};
