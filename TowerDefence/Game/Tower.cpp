#include "Tower.h"
#include <iostream>

Tower::Tower(int x, int y, tStats t, int id, Map &m)
{
	stats = t;
	setX(x);
	setY(y);
	setID(id);
	stats.id = id;

	m.setTower(x - BORDER_SIZE, y - BORDER_SIZE, id);
};

Tower::~Tower()
{

}

void Tower::upgrade()
{
	stats.level++;
	stats.damage *= 2;
}

void Tower::update(Map &m, std::vector<Enemy*> &enemies)
{
	for (int x = getX() - stats.range; (x < getX() + stats.range) && (x < BOARD_WIDTH * BLOCK_SIZE); x += BLOCK_SIZE)
	{
		for (int y = getY() - stats.range; (y < getY() + stats.range) && (y < BOARD_HEIGHT * BLOCK_SIZE); y += BLOCK_SIZE)
		{
			if (m.hasEnemy(x, y))
			{
				for (std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
				{
					if ((*e)->getID() == m.getEnemy(x, y))
					{
						enemy = (*e);
						hasEnemy = true;
						break;
					}
				}
				break;
			}
		}
	}
	
	if (hasEnemy)
	{
		if ((signed)bullets.size() <= stats.maxCapacity)
		{
			Bullet * b = new Bullet((getX() + (BLOCK_SIZE / 2)), (getY() + (BLOCK_SIZE / 2)), stats.range, enemy);
			bullets.push_back(b);
		}
	}
	else
	{
		enemy = NULL;
	}
}

tStats Tower::getStats()const
{
	return stats;
}

void Tower::incrementKills()
{
	stats.kills++;
}