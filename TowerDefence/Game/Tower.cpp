#include "Tower.h"
#include <iostream>

Tower::Tower(int x, int y, int t, int id, Map* m) 
{
	setX(x);
	setY(y);
	setID(id);
	stats.id = id;
	setStats(t);

	(*m).setTerrain(x - BORDER, y - BORDER, HAS_TOWER);
	(*m).setTower(x - BORDER, y - BORDER, id);
};

Tower::~Tower()
{

}

void Tower::setStats(int t)
{
	stats.type = t;
	stats.level = 0;
	stats.hits = 0;
	stats.kills = 0;
	stats.currentlyFired = 0;


	switch (t)
	{

	// Machine Gun
	case 1:
		stats.cost = 50;
		stats.damage = 4;
		stats.range = 4;
		stats.reload = 20;
		stats.maxCapacity = 1;
		break;

	// Light Cannon
	case 2:
		stats.cost = 75;
		stats.damage = 10;
		stats.range = 3;
		stats.reload = 100;
		stats.maxCapacity = 1;
		break;

	// Default
	case 0: default:
		stats.cost = 0;
		stats.damage = 0;
		stats.range = 0;
		stats.reload = 0;
		stats.maxCapacity = 1;
		break;
	}

	stats.range *= BLOCK_SIZE;
}

void Tower::update(Map* m, vector<Enemy*>* enemies)
{
	hasEnemy = checkForEnemies(m, enemies);

	if (hasEnemy)
	{
		fire();
	}

	for (std::vector<Bullet*>::iterator b = bullets.begin(); b != bullets.end(); ++b)
	{
		if ((*b)->expired())
		{
			stats.currentlyFired--;
		}
	}
}

void Tower::fire()
{
	if (stats.currentlyFired <= stats.maxCapacity)
	{
		stats.currentlyFired++;
		Bullet * b = new Bullet((getX() + (BLOCK_SIZE / 2)), (getY() + (BLOCK_SIZE / 2)), stats.range, enemy);
		bullets.push_back(b);
	}
}

int Tower::getCost()
{
	return stats.cost;
}

tStats Tower::getStats()const
{
	return stats;
}

void Tower::incrementKills()
{
	stats.kills++;
}

bool Tower::checkForEnemies(Map* m, vector<Enemy*>* enemies)
{
	for (int x = getX() - stats.range; (x < getX() + stats.range) && (x < BOARD_WIDTH * BLOCK_SIZE); x += BLOCK_SIZE)
	{
		for (int y = getY() - stats.range; (y < getY() + stats.range) && (y < BOARD_HEIGHT * BLOCK_SIZE); y += BLOCK_SIZE)
		{
			if ((*m).getTerrain(x, y) == HAS_ENEMY)
			{
				for (std::vector<Enemy*>::iterator e = enemies->begin(); e != enemies->end(); ++e)
				{
					if ((*e)->getID() == (*m).getEnemy(x, y))
					{
						enemy = (*e);
						return true;
					}
				}
			}
		}
	}
	return false;
}