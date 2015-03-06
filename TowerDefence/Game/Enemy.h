#pragma once
#include "../Engine/Constants.h"
#include "../Engine/Object.h"
#include "../Engine/Pathfinder.h"
#include "Map.h"

struct eStats
{
	int id, targetX, targetY, value, bounty, speed;
	float currentHealth, maxHealth;
	bool isBoss, canSwim;
	std::string category;
};

class Enemy : public Object
{
private:
	Pathfinder astar;
	eStats stats;
	int stepsTaken, stepsPerSquare;
	void initialise(int level, int x, int y, int tX, int tY, int type, Map &m);
	void releaseTiles(Map &m);
	void lockTiles(Map &m);

public:
	Enemy();
	Enemy(ResourceManager &rm, int x, int y, int type, int targetX, int targetY, int level, Map &m, int i);
	~Enemy();

	void updateTarget(int tX, int t, Map &m); 
	void updatePath(Map &m);
	bool reduceHealth(int h, Map &m);

	bool leftBase;

	int getNextMove()const;
	int getSpeed()const;
	eStats getStats()const;
	Pathfinder getPath()const;

	bool canWalk(Map &m);
};

