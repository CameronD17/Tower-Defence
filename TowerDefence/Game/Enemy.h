#pragma once
#include "../Engine/Constants.h"
#include "../Engine/Engine.h"
#include "../Engine/Object.h"
#include "../Engine/Pathfinder.h"
#include "Bank.h"
#include "Map.h"

struct eStats
{
	int id, targetX, targetY, value, bounty, speed, type;
	float currentHealth, maxHealth;
	bool isBoss, canSwim;
	std::string category;
};

class Enemy : public Object
{
private:
	Engine engine;
	Pathfinder astar;
	eStats stats;
	int stepsTaken, stepsPerSquare;
	void initialise(int level, int x, int y, int tX, int tY, int type, Map &m);
	void releaseTiles(Map &m);
	void lockTiles(Map &m);

public:
	Enemy();
	Enemy(Engine &e, int x, int y, int type, int targetX, int targetY, int level, Map &m, int i);
	~Enemy();

	void draw();
	void updateTarget(int tX, int t, Map &m); 
	void updatePath(Map &m, int x = -1, int y = -1);
	bool reduceHealth(int h, Map &m, Bank &b);
	
	void move(Map &m);
};

