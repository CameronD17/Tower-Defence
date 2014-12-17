#include "Tower.h"
#include <iostream>

Tower::Tower() 
{
	setX(0);
    setY(0);
	setType(0);
	damage = 10;
}

Tower::Tower(int x, int y) 
{
	setX(x);
	setY(y);
	setType(0);
}

Tower::Tower(int x, int y, int t) 
{
	setX(x);
	setY(y);
	setType(t);
	setStats(t);
};

Tower::~Tower()
{

}

void Tower::setStats(int t)
{
	level = 0;
	hits = 0;
	kills = 0;
	currentlyFired = 0;

	switch (t)
	{

	// Machine Gun
	case 1:
		cost = 50;
		damage = 4;
		range = 4;
		reload = 20;
		maxCapacity = 1;
		break;

	// Light Cannon
	case 2:
		cost = 75;
		damage = 10;
		range = 3;
		reload = 100;
		maxCapacity = 1;
		break;

	// Default
	case 0: default:
		cost = 0;
		damage = 0;
		range = 0;
		reload = 0;
		maxCapacity = 1;
		break;
	}

	range *= BLOCK_SIZE;
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
		if ((*b)->hitTarget())
		{
			currentlyFired--;
		}
	}
}

void Tower::fire()
{
	if (currentlyFired <= maxCapacity)
	{
		currentlyFired++;
		Bullet * b = new Bullet((getX() + (BLOCK_SIZE / 2)), (getY() + (BLOCK_SIZE / 2)), (enemy->getX() + (BLOCK_SIZE / 2)), (enemy->getY() + (BLOCK_SIZE / 2)), range);
		bullets.push_back(b);
	}
}

int Tower::getCost()
{
	return cost;
}

bool Tower::checkForEnemies(Map* m, vector<Enemy*>* enemies)
{
	for (int x = getX() - range; (x < getX() + range) && (x < BOARD_WIDTH * BLOCK_SIZE); x += BLOCK_SIZE)
	{
		for (int y = getY() - range; (y < getY() + range) && (y < BOARD_HEIGHT * BLOCK_SIZE); y += BLOCK_SIZE)
		{
			if ((*m).getTerrain(x, y) == HASENEMY)
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