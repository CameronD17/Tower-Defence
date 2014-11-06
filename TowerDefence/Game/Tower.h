#pragma once
#include "../Engine/Object.h"
#include "Map.h"

class Tower : public Object
{

public:
	Tower();
	Tower(int x, int y);
	Tower(int x, int y, int t);
	~Tower();

	void fire(long dir);

	bool checkForEnemies(Map* m);

	int getCost();

	

private:
	int x, y;
	bool enemyDetected;
	int type;
	int cost;
	int damage;
	int range;
	int reload;
	
	int level;
	int hits;
	int kills;

	void setStats(int t);
};

