#pragma once
#include "../Engine/Object.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Map.h"

struct tStats
{
	int id, type, cost, damage, range, reload, maxCapacity, currentlyFired, level, hits, kills;
};

class Tower : public Object
{

public:
	Tower() : Object(){};
	Tower(int x, int y) : Object(){};
	Tower(int x, int y, int t, int id);
	~Tower();

	void update(Map* m, vector<Enemy*>* enemies);

	int getCost();
	tStats getStats()const;
	Enemy* enemy;
	vector<Bullet*> bullets;

	bool hasEnemy;	

private:

	tStats stats;
	void setStats(int t);
	void fire();
	bool checkForEnemies(Map* m, vector<Enemy*>* enemies);
};

