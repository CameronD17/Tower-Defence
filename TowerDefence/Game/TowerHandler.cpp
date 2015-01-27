#include "TowerHandler.h"

TowerHandler::TowerHandler(void){}
	
void TowerHandler::init(Map &m)								// Setting up the TowerHandler
{
	towerCount++;
	Tower * t = new Tower(288, 336, 1, towerCount, m);
	m.setTower(264, 312, towerCount);
	towers.push_back(t);
}

int TowerHandler::numTowers()
{
	return towerCount;
}

bool TowerHandler::buildTower(Map &map, Cursor &cursor, Bank &bank)
{
	int x = cursor.getX() - BORDER_SIZE, y = cursor.getY() - BORDER_SIZE;		// Store coordinates to make things easier to read.

	// a) Can the terran be built upon (not blocked)?
	if (map.buildable(x, y))
	{
		// b) Is the player trying to build on the enemy spawn point?
		if (!(x == map.startX && y == map.startY))
		{
			// c) Does the player have enough credit to purchase the tower?
			if (bank.getCredit() >= (10 * cursor.getAction()))
			{
				char terrainReset = map.getTerrain(x, y);			// Store terrain type in case a reset is required
				Pathfinder p;					// Pathfinder to check paths
				map.setTerrain(x, y, HAS_TOWER);					// Set the terrain to blocked in order to check the path

				// d) Does the new tower completely block a route from enemy spawn to player base?
				if (p.findPath(map.startX, map.startY, map.targetX, map.targetY, map))
				{
					towerCount++;
					Tower * t = new Tower(cursor.getX(), cursor.getY(), cursor.getAction(), towerCount, map);
					map.setTower(x, y, towerCount);
					towers.push_back(t);
					bank.decreaseCredit(t->getCost());
					return true;
				}
				else // d)
				{
					map.setTerrain(x, y, terrainReset);
					//setMessage("Invalid placement - you cannot block the route to your base.");
				}
			}
			else // c)
			{
				//setMessage("Insufficient funds available to build.");
			}
		}
		else // b)
		{
			//setMessage("Invalid placement - you cannot build on the enemy spawn point.");
		}
	}
	else // a)
	{
		//setMessage("Invalid placement - this terrain is obstructed.");
	}

	return false;
}

bool TowerHandler::sellTower(Map &m, int id, Bank &b)
{	
	for (std::vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t)
	{
		if ((*t)->getID() == id)
		{
			m.setTerrain((*t)->getX() - BORDER_SIZE, (*t)->getY() - BORDER_SIZE, CLEAR_TERRAIN);
			m.setEnemy((*t)->getX() - BORDER_SIZE, (*t)->getY() - BORDER_SIZE, 0);
			b.increaseCredit((*t)->getCost() / 2);
			(*t)->setDeleted(true);
			//setMessage("Tower sold!");
			return true;
		}
	}

	return false;
}

void TowerHandler::destroyObjects()
{
	for (vector<Tower*>::iterator t = towers.begin(); t != towers.end();)
	{
		for (vector<Bullet*>::iterator b = (*t)->bullets.begin(); b != (*t)->bullets.end();)
		{
			if ((*b)->isDeleted())
			{
				delete *b;
				b = (*t)->bullets.erase(b);
			}
			else b++;
		}

		if ((*t)->isDeleted())
		{
			for (vector<Bullet*>::iterator b = (*t)->bullets.begin(); b != (*t)->bullets.end();)
			{
				delete *b;
				b = (*t)->bullets.erase(b);
			}
			delete *t;
			t = towers.erase(t);
		}
		else t++;
	}
}