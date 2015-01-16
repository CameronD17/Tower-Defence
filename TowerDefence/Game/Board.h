#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "TowerHandler.h"
#include "EnemyHandler.h"
#include "Cursor.h"
#include "Bank.h"
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

	// Timers
	unsigned int eTimer;

	// Per-game values				
	int score, level;
	string message;	

	void cleanup();
	

public:	
	Board(void);

	int startHealth, currentHealth;
	bool debugMode; 

	bool enemySelected, towerSelected, objectSelected;
	eStats selectedEnemyStats;
	tStats selectedTowerStats;

	int getLevel();
	int getScore();

	Engine engine;
	Map map;
	Bank bank;
	TowerHandler towerHandler;
	EnemyHandler enemyHandler;

	void selectObject(Cursor &cursor);
	void deselectObject();

	void setup(Engine &e);
	void update();
};
