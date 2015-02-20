#pragma once
#include "../Engine/Object.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Map.h"

struct tStats
{
	int id, type, cost, damage, range, reload, maxCapacity, level, maxLevel, hits, kills;
};

class Tower : public Object
{
private:
	tStats stats;

public:
	Tower() : Object(){};
	Tower(int x, int y) : Object(){};
	Tower(int x, int y, tStats t, int id, Map &m);
	~Tower();

	void update(Map &m, std::vector<Enemy*> &enemies);

	tStats getStats()const;
	void incrementKills();
	void upgrade();

	Enemy* enemy;
	bool hasEnemy;
	std::vector<Bullet*> bullets;
};

