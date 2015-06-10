#pragma once
#include "../Engine/Object.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Map.h"
#include "Bank.h"

struct tStats
{
	int id, type, cost, damage, range, reload, maxCapacity, level, maxLevel, hits, kills;
};

class Tower : public Object
{
private:
	Engine engine;
	tStats stats;

public:
	Tower() : Object(){};
	Tower(int x, int y) : Object(){};
	Tower(Engine &e, int x, int y, int t, int id, Map &m);
	~Tower();

	void update(Map &m, Bank &b, std::vector<boost::shared_ptr<Enemy>> &enemies);

	tStats getStats()const;
	void upgrade();

	boost::shared_ptr<Enemy> enemy;
	bool hasEnemy;
	std::vector<Bullet*> bullets;
};