#include "TowerHandler.h"

TowerHandler::TowerHandler(void){}
	
void TowerHandler::init(Map* m)								// Setting up the TowerHandler
{
	towerCount++;
	Tower * t = new Tower(288, 336, 1, towerCount, m);
	(*m).setTower(264, 312, towerCount);
	towers.push_back(t);
}

int TowerHandler::numTowers()
{
	return towerCount;
}

bool TowerHandler::buildTower(Map* m, Cursor &cursor, Bank* b)
{
	int x = cursor.getX() - BORDER, y = cursor.getY() - BORDER;		// Store coordinates to make things easier to read.

	// a) Can the terran be built upon (not blocked)?
	if ((*m).buildable(x, y))
	{
		// b) Is the player trying to build on the enemy spawn point?
		if (!(x == (*m).startX && y == (*m).startY))
		{
			// c) Does the player have enough credit to purchase the tower?
			if (b->getCredit() >= (10 * cursor.getAction()))
			{
				char terrainReset = (*m).getTerrain(x, y);			// Store terrain type in case a reset is required
				Pathfinder* p = new Pathfinder();					// Pathfinder to check paths
				(*m).setTerrain(x, y, HAS_TOWER);					// Set the terrain to blocked in order to check the path

				// d) Does the new tower completely block a route from enemy spawn to player base?
				if (p->findPath((*m).startX, (*m).startY, (*m).targetX, (*m).targetY, m))
				{
					towerCount++;
					Tower * t = new Tower(cursor.getX(), cursor.getY(), cursor.getAction(), towerCount, m);
					(*m).setTower(x, y, towerCount);
					towers.push_back(t);
					b->decreaseCredit(t->getCost());
					return true;
				}
				else // d)
				{
					(*m).setTerrain(x, y, terrainReset);
					//setMessage("Invalid placement - you cannot block the route to your base.");
				}

				delete p;
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

bool TowerHandler::sellTower(Map* m, int id, Bank* b)
{	
	for (std::vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t)
	{
		if ((*t)->getID() == id)
		{
			(*m).setTerrain((*t)->getX() - BORDER, (*t)->getY() - BORDER, CLEAR_TERRAIN);
			(*m).setEnemy((*t)->getX() - BORDER, (*t)->getY() - BORDER, 0);
			b->increaseCredit((*t)->getCost() / 2);
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
			delete *t;
			t = towers.erase(t);
		}
		else t++;
	}
}