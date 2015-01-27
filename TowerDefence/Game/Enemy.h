#pragma once
#include "../Engine/Constants.h"
#include "../Engine/Object.h"
#include "../Engine/Pathfinder.h"
#include "Map.h"
#include <vector>

struct eStats
{
	int id, targetX, targetY, currentHealth, maxHealth, value, bounty, speed;
	bool leftBase, isBoss, canSwim, isWaiting;
	string category;
};

class Enemy : public Object
{

public:
	Enemy();
	Enemy(ResourceManager &rm, int x, int y, int type, int targetX, int targetY, int level, Map &m, int i);
	~Enemy();

	void updateTarget(int tX, int t, Map &m); 
	void updatePath(Map &m);
	bool reduceHealth(int h, Map &m);

	int getNextMove()const;
	int getSpeed()const;
	eStats getStats()const;
	Pathfinder getPath()const;

	bool canWalk(Map &m);	

private:

	Pathfinder astar;
	eStats stats;

	int stepsTaken, stepsPerSquare;

	void initialise(int level, int x, int y, int tX, int tY, int type, Map &m);

	void releaseAllMyTiles(Map &m);
	void lockThisTile(Map &m);
	void lockNextTile(Map &m);
};

