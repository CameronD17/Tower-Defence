#pragma once

#include "../Engine/Engine.h"
#include "Constants.h"
#include "Tower.h"
#include "Enemy.h"
#include "Cursor.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <fstream>	

using namespace std;

class Board 
{

private:
	// *** VARIABLES *** //

	// Timers
	unsigned int eTimer;

	

	// Per-game values				
	int score, credit, level, nextEnemy;
	string message;	
	string initials;
	bool hasEnemy[BOARD_WIDTH][BOARD_HEIGHT];
	

	// *** METHODS *** //

	// Gameplay methods
	void buildTower(Cursor &cursor);
	bool checkTowerPlacement(Cursor &cursor);
	void deleteTower(Cursor &cursor);

	void launchEnemy(Cursor &cursor);
	void moveEnemies();
	void moveBullets();
	void destroyObjects();
	void cleanup();

public:	
	Board(void);

	// Initial values for game setup
	int startHealth, currentHealth;
	int targetX, targetY;				
	int startX, startY;	
	int type, enemyCount;
	bool debugMode;

	Engine engine;
	Map map;

	vector<Tower*>  towers;
	vector<Enemy*>  enemies;

	void setup(Engine &e);
	int getInput(Cursor &cursor);
	void update();
	void draw(float interpolation);
};
