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
	unsigned int eTimer;			
	int level;
	
public:	
	Board(void);

	int startHealth, currentHealth;
	bool debugMode; 
	int getLevel();

	Engine engine;
	Map map;
	Bank bank;
	TowerHandler towerHandler;
	EnemyHandler enemyHandler;

	void deselectObject();

	void setup(Engine &e, std::string g);
	int getInput(input k, Cursor &c, Notification& n);
	void update();
	void draw();
};
