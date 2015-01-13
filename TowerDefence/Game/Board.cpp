#include "Board.h"

Board::Board(void){}
	
void Board::setup(Engine &e)								// Setting up the Board
{
	engine = e;			
	startX = 24, startY = 24, targetX = 384, targetY = 408;
	map.init(0);
	eTimer = SDL_GetTicks() + 300;
	enemyCount = 1, towerCount = 0;
	credit = 100000; 

	Enemy * enemy = new Enemy(engine.resources,	startX, startY,	type, targetX, targetY, 1, &map, enemyCount);
	enemies.push_back(enemy);
}



// *** UPDATE METHODS *** //

void Board::update()
{
	if (SDL_GetTicks() > eTimer)
	{
		for (std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
		{
			if ((*e)->canWalk(&map))
			{
				engine.physics.move((*e), (*e)->nextMove(), (*e)->getSpeed());
			}
		}

		for (std::vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t)
		{
			(*t)->update(&map, &enemies);

			for (std::vector<Bullet*>::iterator b = (*t)->bullets.begin(); b != (*t)->bullets.end(); ++b)
			{
				engine.physics.nonUniformMove((*b), (*b)->dX, (*b)->dY);
			}
		}

		eTimer = SDL_GetTicks() + 0;
	}

	cleanup();
}

int Board::getInput(Cursor &cursor)
{
	if (cursor.getTowerType() == 0)
	{
		targetX = cursor.getX(); targetY = cursor.getY();

		for (vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
		{
			(*e)->updateTarget(targetX, targetY, &map);
		}
	}
	else if (cursor.getTowerType() == 1)
	{
		buildTower(cursor);
	}
	else if (cursor.getTowerType() == 2)
	{
		launchEnemy(cursor);
	}
	else
	{
		checkForObject(cursor);
	}

	return 0;
}

void Board::checkForObject(Cursor &cursor)
{
	int x = cursor.getX() - BORDER, y = cursor.getY() - BORDER;

	if (map.getTerrain(x, y) == HASENEMY)
	{
		objectSelected = true;
		enemySelected = true;
		towerSelected = false;

		selectedEnemyStats = getEnemyFromId(map.getEnemy(x, y))->getStats();
	}
	else if (map.getTerrain(x, y) == HASTOWER)
	{
		objectSelected = true;
		enemySelected = false;
		towerSelected = true;

		selectedTowerStats = getTowerFromId(map.getTower(x, y))->getStats();
	}
}

eStats Board::pullEnemyStats()
{		
	return selectedEnemyStats;
}

tStats Board::pullTowerStats()
{
	return selectedTowerStats;
}

Enemy* Board::getEnemyFromId(int id)
{
	for (std::vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		if ((*e)->getID() == id)
		{
			return (*e);
		}
	}

	return NULL;
}

Tower* Board::getTowerFromId(int id)
{
	for (std::vector<Tower*>::iterator t = towers.begin(); t != towers.end(); ++t)
	{
		if ((*t)->getID() == id)
		{
			return (*t);
		}
	}

	return NULL;
}

void Board::launchEnemy(Cursor &cursor)
{
	enemyCount++;
	Enemy * e = new Enemy(engine.resources, cursor.getX(), cursor.getY(), 0, targetX, targetY, 1, &map, enemyCount);
	enemies.push_back(e);
}

void Board::buildTower(Cursor &cursor)
{
	if (checkTowerPlacement(cursor))
	{
		Tower * t = new Tower(cursor.getX(), cursor.getY(), cursor.getTowerType(), towerCount);
		towers.push_back(t);
		//credit -= (t->getCost());

		/* Update the enemy path if and only if the tower being placed is on their path */
		for (vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
		{
			for (unsigned i = (*e)->astar.pathToFollow.size(); i > 0; i--)
			{
				int pathX = ((*e)->astar.xCoordinates.at(i)*BLOCK_SIZE) + BORDER;
				int pathY = ((*e)->astar.yCoordinates.at(i)*BLOCK_SIZE) + BORDER;

				if (pathX == t->getX() && pathY == t->getY())
				{
					(*e)->updatePath(&map);
					break;
				}
			}
		}
	}
}

bool Board::checkTowerPlacement(Cursor &cursor)
{    
	int x = cursor.getX() - BORDER, y = cursor.getY() - BORDER;		// Store coordinates to make things easier to read.

	// a) Can the terran be built upon (not blocked)?
	if (map.buildable(x, y))
	{	
		// b) Is the player trying to build on the enemy spawn point?
		if (!(x == startX && y == startY))
		{
			// c) Does the player have enough credit to purchase the tower?
			if (credit >= (10 * cursor.getTowerType()))
			{
				char terrainReset = map.getTerrain(x, y);			// Store terrain type in case a reset is required
				Pathfinder p;										// Pathfinder to check paths
				map.setTerrain(x, y, HASTOWER);						// Set the terrain to blocked in order to check the path
				towerCount++;
				map.setTower(x, y, towerCount);

				// d) Does the new tower completely block a route from enemy spawn to player base?
				if (p.findPath(startX, startY, targetX, targetY, &map))
				{
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

void Board::cleanup()
{
	destroyObjects();
}

void Board::destroyObjects()
{
	for (vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end();)
	{
		if ((*e)->isDeleted())
		{
			delete *e;
			e = enemies.erase(e);
		}
		else e++;
	}

	for (vector<Tower*>::iterator t = towers.begin(); t != towers.end(); t++)
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
	}
}