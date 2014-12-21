#include "Board.h"

Board::Board(void){}
	
void Board::setup(Engine &e)								// Setting up the Board
{
	engine = e;			
	startX = 24, startY = 24, targetX = 384, targetY = 408;
	map.init(0);
	//srand((unsigned int)time(NULL));	
	eTimer = SDL_GetTicks() + 300;
	enemyCount = 1;

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

	destroyObjects();
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
	else if (cursor.getTowerType() == 3)
	{
		launchEnemy(cursor);
	}
	else
	{
		placeTower(cursor);
	}

	return 0;
}

void Board::launchEnemy(Cursor &cursor)
{
	enemyCount++;
	Enemy * e = new Enemy(engine.resources, cursor.getX(), cursor.getY(), 0, targetX, targetY, 1, &map, enemyCount);
	enemies.push_back(e);
}

void Board::placeTower(Cursor &cursor)
{    
	/* Only build a tower if:  
	 * a) The terrain can be built upon
	 * b) The player has enough credit to build the tower
	 * c) The tower is not blocking the spawn point of the enemies
	 * d) The tower does not completely block a route from the enemy spawn point to the base
	 */
	int xPos = cursor.getX() - BORDER, yPos = cursor.getY() - BORDER;

	if (clearToBuild(xPos, yPos))
	{
		if (pathAvailable(xPos, yPos))
		{
			//if (credit >= (10 * cursor.getType()))
			//{
				Tower * t = new Tower(cursor.getX(), cursor.getY(), cursor.getTowerType());
				towers.push_back(t);
				//credit -= (t->getCost());
			//}
			//else
			//{
			//	setMessage("Insufficient funds available to build.");
			//}
		}
		else
		{
			//setMessage("Invalid placement - you cannot block the route to your base.");
		}
	}
	else
	{
		//setMessage("Invalid placement - this terrain is obstructed.");
	}
}

bool Board::clearToBuild(int x, int y)
{
	return (map.getTerrain(x, y) == CLEARTERRAIN || map.getTerrain(x, y) == ROUGHTERRAIN);
}

bool Board::pathAvailable(int x, int y)
{	
	char terrainReset = map.getTerrain(x, y);
	map.setTerrain(x, y, BLOCKEDTERRAIN);

	// No path found from base to target
	Pathfinder p;
	if (!p.findPath(24, 24, targetX, targetY, &map))
	{
		map.setTerrain(x, y, terrainReset);
		return false;
	}

	// Path available, update enemies ONLY IF the tower placed is on their current path
	for (vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		for (unsigned i = (*e)->astar.pathToFollow.size(); i > 0; i--)
		{
			if ((*e)->astar.xCoordinates.at(i)*BLOCK_SIZE == x && (*e)->astar.xCoordinates.at(i)*BLOCK_SIZE == y)
			{
				(*e)->astar.findPath((*e)->getX(), (*e)->getY(), targetX, targetY, &map);
				break;
			}
		}
	}

	return true;
}

void Board::cleanup()
{

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