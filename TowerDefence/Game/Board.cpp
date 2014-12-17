#include "Board.h"

Board::Board(void){}
	
void Board::setup(Engine &e)								// Setting up the Board
{
	engine = e;
	cursor.init(engine.resources);				
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

int Board::getInput()
{
	input k = engine.interfaces.getInput();	// Get input from the user

	if (k.keyPress)
	{
		if (k.key == SDLK_ESCAPE)	// Quit the Board
		{
			return -1;
		}
		else if (k.key == SDLK_0)
		{
			cursor.changeTowerType(0);
		}
		else if (k.key == SDLK_1)
		{
			cursor.changeTowerType(1);
		}
		else if (k.key == SDLK_2)
		{
			cursor.changeTowerType(2);
		}
		else if (k.key == SDLK_3)
		{
			cursor.changeTowerType(3);
		}
		else if (k.key == SDLK_d)
		{
			debugMode ? debugMode = false : debugMode = true;
		}
		else
		{
			cursor.changeTowerType(1);
		}
	}
	else if (k.mouseDown)
	{
		if (cursor.getType() == 0)
		{
			targetX = cursor.getX(); targetY = cursor.getY();
			for (vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
			{
				(*e)->updateTarget(targetX, targetY);
				(*e)->updatePath(&map);
			}
		}
		else if (cursor.getType() == 3)
		{
			launchEnemy();
		}
		else
		{
			placeTower();
		}
	}
	else
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		if (x > BORDER && x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE)) && y > BORDER && y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE)))
		{
			//cursor.setType(1);
			cursor.setX((x - (x%BLOCK_SIZE)));
			cursor.setY((y - (y%BLOCK_SIZE)));
		}
		else 
		{
			if (cursor.getType() != 0)
			{
				cursor.setType(0);
				if (x > BORDER && (y > BORDER && y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE))))
				{
					cursor.setX((BOARD_WIDTH*BLOCK_SIZE) - BLOCK_SIZE * 2);
				}
				else if (x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE)) && (y > BORDER && y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE))))
				{
					cursor.setX(BORDER - BLOCK_SIZE);
				}

				if (y > BORDER && (x > BORDER && x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE))))
				{
					cursor.setY((BOARD_HEIGHT*BLOCK_SIZE) - BLOCK_SIZE * 2);
				}
				else if (y < (BORDER + (BOARD_HEIGHT*BLOCK_SIZE))&& (x > BORDER && x < (BORDER + (BOARD_WIDTH*BLOCK_SIZE))))
				{
					cursor.setY(BORDER - BLOCK_SIZE);
				}
			}
		}			
	}

    return 0;
}

void Board::launchEnemy()
{
	enemyCount++;
	Enemy * e = new Enemy(engine.resources, cursor.getX(), cursor.getY(), 0, targetX, targetY, 1, &map, enemyCount);
	enemies.push_back(e);
}

void Board::placeTower()
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
				Tower * t = new Tower(cursor.getX(), cursor.getY(), cursor.getType());
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
			setMessage("Invalid placement - you cannot block the route to your base.");
		}
	}
	else
	{
		setMessage("Invalid placement - this terrain is obstructed.");
	}
}

bool Board::clearToBuild(int x, int y)
{
	char	s1 = map.getTerrain(x, y);
			/*s2 = map.getTerrain(x + BLOCK_SIZE, y),
			s3 = map.getTerrain(x, y + BLOCK_SIZE), 
			s4 = map.getTerrain(x + BLOCK_SIZE, y + BLOCK_SIZE);*/

	if (	(s1 == CLEARTERRAIN || s1 == ROUGHTERRAIN)/*
		&&	(s2 == CLEARTERRAIN || s2 == ROUGHTERRAIN)
		&&	(s3 == CLEARTERRAIN || s3 == ROUGHTERRAIN)
		&&	(s4 == CLEARTERRAIN || s4 == ROUGHTERRAIN)*/)
	{
		return true;
	}

	return false;
}

bool Board::pathAvailable(int x, int y)
{
	// Check that the new tower wouldn't be blocking the spawn point of enemies
	if ((cursor.getX() + ((1 + cursor.getType())*BLOCK_SIZE) > startX &&	cursor.getY() - ((cursor.getType())*BLOCK_SIZE) < startY))
	{
		return false;
	}
	
	char terrainReset[4];
	bool pathAvailable = true;
	
	terrainReset[0] = map.getTerrain(x, y)/*, terrainReset[1] = map.getTerrain(x + BLOCK_SIZE, y),
	terrainReset[2] = map.getTerrain(x, y + BLOCK_SIZE), terrainReset[3] = map.getTerrain(x + BLOCK_SIZE, y + BLOCK_SIZE)*/;

	map.setTerrain(x, y, BLOCKEDTERRAIN);
	/*map.setTerrain(x + BLOCK_SIZE, y, BLOCKEDTERRAIN);
	map.setTerrain(x, y + BLOCK_SIZE, BLOCKEDTERRAIN);
	map.setTerrain(x + BLOCK_SIZE, y + BLOCK_SIZE, BLOCKEDTERRAIN);*/
	
	/*
	This method may well cause bugs later on in development; it may be possible to place a tower behind the last enemy
	in a wave, blocking off all future waves but none of the existing enemies. MAY REQUIRE A REWRITE (test enemy made and
	destroyed, perhaps?)
	*/

	for (vector<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); ++e)
	{
		if (!(*e)->checkPathFromBase(&map))
		{
			pathAvailable = false;

			map.setTerrain(x, y, terrainReset[0]);
			/*map.setTerrain(x + BLOCK_SIZE, y, terrainReset[1]);
			map.setTerrain(x, y + BLOCK_SIZE, terrainReset[2]);
			map.setTerrain(x + BLOCK_SIZE, y + BLOCK_SIZE, terrainReset[3]);*/

			setMessage("Invalid placement - path blocked");
		}		
		(*e)->updatePath(&map);
	}

	return pathAvailable;
}

void Board::cleanup()
{

}

void Board::setMessage(string m)
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