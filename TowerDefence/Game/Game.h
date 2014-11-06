#pragma once

#include "../Engine/Engine.h"
#include "Constants.h"
#include "Cursor.h"
#include "Tower.h"
#include "Enemy.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <fstream>	

using namespace std;

class Game 
{

private:
	// *** VARIABLES *** //

	// Timers
	unsigned int eTimer;

	// Initial values for game setup
	int startHealth, currentHealth;
	int targetX, targetY;				
	int startX, startY;	
	int type, enemyCount;

	// Per-game values				
	int score, credit, level, nextEnemy;
	string message;	
	string initials;
	bool hasEnemy[BOARD_WIDTH][BOARD_HEIGHT];
	bool debugMode;
	
	// Game pieces
	Map map;
	Cursor cursor;
	vector<Tower*>  towers;
	vector<Enemy*>  enemies;
	
	
	// *** METHODS *** //
			

	// Draw methods
	void drawBoardBackground();
	void drawGamePieces();
	void drawBoardForeground();
	void drawCursor();
	void drawDebugFeatures();

	// Gameplay methods
	void placeTower();
	void launchEnemy();
	void deleteTower();
	void moveEnemies();
	void moveBullets();
	void cleanup();
	void saveScore();
	void setMessage(string m);

	bool pathAvailable(int xPos, int yPos);
	bool clearToBuild(int xPos, int yPos);

public:	
	Game(void);
	Game(Engine &sc);	
	Engine engine;
	
	void newGame();
	int getInput();
	void update();
	void draw(float interpolation);
};
