#pragma once
#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "TowerHandler.h"
#include "EnemyHandler.h"
#include "Cursor.h"
#include "Bank.h"
#include "Notification.h"

class Board 
{
private:			
	int level;

public:	
	Board(void);

	int startHealth, currentHealth;
	bool debugMode; 

	Engine engine;
	Map map;
	Bank bank;
	TowerHandler towerHandler;
	EnemyHandler enemyHandler;

	void setup(Engine &e, std::string g);
	void getInput(input k, Cursor &c, Notification& n);
	void update();
	void draw();
};
