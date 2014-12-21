#pragma once
#include "../Engine/Object.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Map.h"


class Tower : public Object
{

public:
	Tower() : Object(){};
	Tower(int x, int y) : Object(){};
	Tower(int x, int y, int t);
	~Tower();

	void update(Map* m, vector<Enemy*>* enemies);

	int getCost();
	Enemy* enemy;
	vector<Bullet*> bullets;

	bool hasEnemy;	

private:
	int type;
	int cost;
	int damage;
	int range;
	int reload;
	int maxCapacity;
	int currentlyFired;
	
	int level;
	int hits;
	int kills;

	void setStats(int t);
	void fire();
	bool checkForEnemies(Map* m, vector<Enemy*>* enemies);
};

