#include "Enemy.h"

Enemy::Enemy()
{
    setX(0);
    setY(0);
	setType(0);
	targetX = 0;
	targetY = 0;
	currentHealth = 30;
	value = 150;
	bounty = 20;
	stepsTaken = 0;
	id = 0;
	waitingPeriod = 0;
}

Enemy::Enemy(ResourceManager &rm, int x, int y, int t, int tX, int tY, int l, Map* m, int i)
{
	resource = rm;
	id = i;
	
	leftBase = false;
	stepsTaken = 0;
	waitingPeriod = 0;

	initialise(x, y, tX, tY, t, m);
	setStats(l);
}

Enemy::~Enemy()
{

}

void Enemy::initialise(int x, int y, int tX, int tY, int t, Map* m)
{
	setX(x);
	setY(y);
	setType(t);

	updateTarget(tX, tY);
	updatePath(m);

	(*m).setTerrain(x - BORDER, y - BORDER, HASENEMY);
	(*m).setEnemy(x - BORDER, y - BORDER, id);
}

void Enemy::setStats(int level)
{
	maxHealth = level * HEALTH_MULTIPLIER;
	currentHealth = maxHealth;
	value = level * VALUE_MULTIPLIER;
	bounty = level * BOUNTY_MULTIPLIER;
	canSwim = false;		// Change to check type
	speed = 0;				// Change to check type
	stepSize = 2;
	stepsPerSquare = BLOCK_SIZE / stepSize;
}

void Enemy::updateTarget(int x, int y)
{
	targetX = (x - BORDER) / BLOCK_SIZE;
	targetY = (y - BORDER) / BLOCK_SIZE;
}

void Enemy::updatePath(Map* m)
{
	setX(getX() - getX() % BLOCK_SIZE);
	setY(getY() - getY() % BLOCK_SIZE);

	(*m).setTerrain(getX() - BORDER, getY() - BORDER, CLEARTERRAIN);

	stepsTaken = 0;
	findPath(getX(), getY(), m);
}

int Enemy::getID()const
{
	return id;
}

int Enemy::getStepSize()const
{
	return stepSize;
}

void Enemy::getSprites()
{
	switch(getType())
	{
	case 1: default:
		sprites.push_back(resource.getImage("Game/Images/Sprites/enemyUp.png"));
		sprites.push_back(resource.getImage("Game/Images/Sprites/enemyDown.png"));
		sprites.push_back(resource.getImage("Game/Images/Sprites/enemyLeft.png"));
		sprites.push_back(resource.getImage("Game/Images/Sprites/enemyRight.png"));
		break;
	case 2:
		sprites.push_back(resource.getImage("Game/Images/Sprites/jetUp.png"));
		sprites.push_back(resource.getImage("Game/Images/Sprites/jetDown.png"));
		sprites.push_back(resource.getImage("Game/Images/Sprites/jetLeft.png"));
		sprites.push_back(resource.getImage("Game/Images/Sprites/jetRight.png"));
		break;
	}
}

int Enemy::getMapX()const
{
	return (getX() - BORDER) / BLOCK_SIZE;
}

int Enemy::getMapY()const
{
	return (getY() - BORDER) / BLOCK_SIZE;
}

int Enemy::getNextX()const
{
	switch (pathToFollow.back())
	{
	case UP: case DOWN:
		return (xCoordinates.back() * BLOCK_SIZE);
	case UPRIGHT: case RIGHT: case DOWNRIGHT:
		return ((xCoordinates.back() + 1) * BLOCK_SIZE);
	case UPLEFT: case LEFT: case DOWNLEFT:
		return ((xCoordinates.back() - 1) * BLOCK_SIZE);
	}
	return -1;
}

int Enemy::getNextY()const
{
	switch (pathToFollow.back())
	{
	case LEFT: case RIGHT:
		return (yCoordinates.back() * BLOCK_SIZE);
	case DOWNLEFT: case DOWN: case DOWNRIGHT:
		return ((yCoordinates.back() + 1) * BLOCK_SIZE);
	case UPLEFT: case UP: case UPRIGHT:
		return ((yCoordinates.back() - 1) * BLOCK_SIZE);
	}
	return -1;
}

bool Enemy::canWalk(Map* map)
{
	// Check if you're there first to avoid unnecessary calculations
	if (reachTarget(map)) return false;

	// Hey, you've committed to moving into the next square. Trust your instincts.
	if (stepsTaken > 0 && stepsTaken < stepsPerSquare)
	{
		stepsTaken++;
		return true;
	}

	// Ooh, time to try and move into a new square. Just make sure you haven't reached the end of your path though. That would make everyone look silly.
	else if (nextMove() != 0)
	{
		// Okay, we haven't reached the base yet. Pop-off the path coordinates of the current tile to access the next tile
		if (stepsTaken == stepsPerSquare)
		{
			pathToFollow.pop_back();
			xCoordinates.pop_back();
			yCoordinates.pop_back();
		}

		// Let's make sure that the next square isn't occupied.
		if (map->walkable(getNextX(), getNextY(), id))
		{
			// GET IN! The next tile is available. Let's move!
			moveIntoNewTile(map);
			return true;
		}
		
		// Well shit, we can't move. Is there some other way we can get around to the base?
		// Let's hope so. We'll update the path with the new values and try again next move.
		else
		{
			updatePath(map);
			return false;
		}
	}

	// You've reached the end of your path! No need to move :) let's just make sure there isn't a better way.
	updatePath(map);
	holdPosition(map);
	return false;	
}

int Enemy::nextMove()
{
	int nextDir = (pathToFollow.size() > 0) ?  pathToFollow.back() : 0;

	return nextDir;
}

void Enemy::moveIntoNewTile(Map* map)
{
	releaseAllMyTiles(map);
	lockThisTile(map);
	lockNextTile(map);
	stepsTaken = 1;	
}

void Enemy::holdPosition(Map* map)
{
	releaseAllMyTiles(map);
	lockThisTile(map);
}

void Enemy::releaseAllMyTiles(Map* map)
{
	/* This is too brute-forcey, should be able to optimise this! */

	for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x += BLOCK_SIZE)
	{
		for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y += BLOCK_SIZE)
		{
			if (map->getEnemy(x, y) == id)
			{
				map->setTerrain(x, y, CLEARTERRAIN);
				map->setEnemy(x, y, 0);
			}
		}
	}
}

void Enemy::lockThisTile(Map* map)
{
	int thisX = getX() - (getX() % BLOCK_SIZE) - BORDER;
	int thisY = getY() - (getY() % BLOCK_SIZE) - BORDER;

	if (map->walkable(thisX, thisY, id))
	{
		map->setTerrain(thisX, thisY, HASENEMY);
		map->setEnemy(thisX, thisY, id);
	}
}

void Enemy::lockNextTile(Map* map)
{
	int thisX = getX() - (getX() % BLOCK_SIZE) - BORDER;
	int thisY = getY() - (getY() % BLOCK_SIZE) - BORDER;
	int otherX = thisX;
	int otherY = thisY;	

	switch (pathToFollow.back())
	{
	case UP: 
		thisY -= BLOCK_SIZE;
		break;
	case RIGHT:
		thisX += BLOCK_SIZE;
		break;
	case DOWN:
		thisY += BLOCK_SIZE;
		break;
	case LEFT:
		thisX -= BLOCK_SIZE;
		break;
	case UPLEFT:
		thisX -= BLOCK_SIZE;
		thisY -= BLOCK_SIZE;
		break;
	case UPRIGHT:
		thisX += BLOCK_SIZE;
		thisY -= BLOCK_SIZE;
		break;
	case DOWNRIGHT:
		thisX += BLOCK_SIZE;
		thisY += BLOCK_SIZE;
		break;
	case DOWNLEFT:
		thisX -= BLOCK_SIZE;
		thisY += BLOCK_SIZE;
		break;
	default:
		break;
	}

	if (map->walkable(thisX, thisY, id))
	{
		map->setTerrain(thisX, thisY, HASENEMY);
		map->setEnemy(thisX, thisY, id);
	}

	if (pathToFollow.back() == UPRIGHT || pathToFollow.back() == UPLEFT || pathToFollow.back() == DOWNRIGHT || pathToFollow.back() == DOWNLEFT)
	{
		if (map->walkable(otherX, thisY, id))
		{
			map->setTerrain(otherX, thisY, HASENEMY);
			map->setEnemy(otherX, thisY, id);
		}

		if (map->walkable(thisX, otherY, id))
		{
			map->setTerrain(thisX, otherY, HASENEMY);
			map->setEnemy(thisX, otherY, id);
		}
	}
}

bool Enemy::checkWalkabilityOfNextTile(Map* map)
{
	bool isFree = false;

	switch (pathToFollow.back())
	{
	case UP: case DOWN: case LEFT: case RIGHT:
		isFree = (map->walkable(getNextX(), getNextY(), id));
		break;

	case UPRIGHT: 
		isFree = (	map->walkable(getNextX(), getNextY(), id) 
			&& map->walkable(getNextX(), getNextY() - BLOCK_SIZE, id) 
			&& map->walkable(getNextX() + BLOCK_SIZE, getNextY(), id));
		break;

	case UPLEFT:
		isFree = (map->walkable(getNextX(), getNextY(), id)
			&& map->walkable(getNextX(), getNextY() - BLOCK_SIZE, id)
			&& map->walkable(getNextX() - BLOCK_SIZE, getNextY(), id));
		break;

	case DOWNRIGHT:
		isFree = (map->walkable(getNextX(), getNextY(), id)
			&& map->walkable(getNextX(), getNextY() + BLOCK_SIZE, id)
			&& map->walkable(getNextX() + BLOCK_SIZE, getNextY(), id));
		break;

	case DOWNLEFT:
		isFree = (map->walkable(getNextX(), getNextY(), id)
			&& map->walkable(getNextX(), getNextY() + BLOCK_SIZE, id)
			&& map->walkable(getNextX() - BLOCK_SIZE, getNextY(), id));
		break;

	default:
		break;
	}

	return isFree;
}

bool Enemy::reachTarget(Map* map)
{
	if (targetX == getMapX() && targetY == getMapY())
	{
		setDeleted(true);
		releaseAllMyTiles(map);
		return true;
	}
	return false;
}


// *** A-Star Pathfinding functions *** //

bool Enemy::checkPathFromBase(Map* m)
{
	bool available = findPath(startX, startY, m);
	return available;
}

bool Enemy::findPath(int sX, int sY, Map* map)
{
	// Initialise values
	aStarSetMapValues(map, canSwim);

	startX = (sX - BORDER) / BLOCK_SIZE, startY = (sY - BORDER) / BLOCK_SIZE;

	int openListID = 0;
	bool pathFound = false;
	pathToFollow.clear();
	xCoordinates.clear();
	yCoordinates.clear();

	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
			whichList[x][y] = UNCHECKED;
	}
	Gcost[startX][startY] = 0;
	openListSize = 1;
	openList[1] = 1;
	openX[1] = startX; openY[1] = startY;

	if (startX == targetX && startY == targetY)	return pathFound;

	while (!pathFound && openListSize != 0)
	{
		parentXval = openX[openList[1]];
		parentYval = openY[openList[1]];
		whichList[parentXval][parentYval] = CLOSED;
		aStarBinaryHeap();

		for (int y = parentYval - 1; y <= parentYval + 1; y++)
		{
			for (int x = parentXval - 1; x <= parentXval + 1; x++)
			{
				if (x > -1 && y > -1 && x < BOARD_WIDTH + 1 && y < BOARD_HEIGHT + 1		// If the tile is not off the map
					&& whichList[x][y] != CLOSED										// and if not already on the closed list
					&& terrain[x][y] != BLOCKEDTERRAIN									// and terrain is not impassable
					&& aStarCutCorner(x, y))											// and you aren't cutting a corner
				{
					//	If not already on the open list, add it to the open list.			
					if (whichList[x][y] != OPEN)
					{
						whichList[x][y] = OPEN;
						openListID++;
						openListSize++;
						openList[openListSize] = openListID;
						openX[openListID] = x;
						openY[openListID] = y;
						Gcost[x][y] = aStarGetGCost(x, y, parentXval, parentYval);
						Fcost[openList[openListSize]] = Gcost[x][y] + ORTHOGONALCOST * (abs(x - targetX) + abs(y - targetY));
						parentX[x][y] = parentXval; parentY[x][y] = parentYval;
						aStarBubbleNewF(openListSize);

					}
					// If it is on the open list, check to see if this is a better path
					else
					{
						if (aStarGetGCost(x, y, parentXval, parentYval) < Gcost[x][y])
						{
							parentX[x][y] = parentXval;
							parentY[x][y] = parentYval;
							Gcost[x][y] = aStarGetGCost(x, y, parentXval, parentYval);

							for (int i = 1; i <= openListSize; i++)
							{
								if (openX[openList[i]] == x && openY[openList[i]] == y)
								{
									Fcost[openList[i]] = Gcost[x][y] + ORTHOGONALCOST * (abs(x - targetX) + abs(y - targetY));
									aStarBubbleNewF(i);
									break;
								}
							}
						}
					}
				}
			}
		}

		if (whichList[targetX][targetY] == OPEN)		pathFound = true;
	}

	if (pathFound)	aStarCalcPath();

	return pathFound;
}

void Enemy::aStarCalcPath()
{
	int x = targetX, y = targetY;

	xCoordinates.push_back(x);
	yCoordinates.push_back(y);

	while (x != startX || y != startY)
	{
		int pX = parentX[x][y], pY = parentY[x][y];

		if (pY < y)				// Previous step was north...
		{
			if (pX < x)			// ...west...
			{
				pathToFollow.push_back(DOWNRIGHT);	// ...so we go south-east
			}
			else if (pX > x)	// ...east
			{
				pathToFollow.push_back(DOWNLEFT);		// ...so we go south-west
			}
			else				// ...exactly
			{
				pathToFollow.push_back(DOWN);			// ...so we go south
			}
		}
		else if (pY > y)		// Previous step was south...
		{
			if (pX < x)			// ...west
			{
				pathToFollow.push_back(UPRIGHT);		// ...so we go north-east
			}
			else if (pX > x)	// ...east
			{
				pathToFollow.push_back(UPLEFT);		// ...so we go north-west
			}
			else				// ...exactly
			{
				pathToFollow.push_back(UP);			// ...so we go north
			}
		}
		else if (pX < x)		// Previous step was west
		{
			pathToFollow.push_back(RIGHT);			// ...so we go east
		}
		else if (pX > x)		// Previous step was east
		{
			pathToFollow.push_back(LEFT);				// ...so we go west
		}

		int tempx = parentX[x][y];
		y = parentY[x][y];
		x = tempx;

		xCoordinates.push_back(x);
		yCoordinates.push_back(y);
	}

}

int Enemy::aStarGetGCost(int x, int y, int px, int py)
{
	int gCost, terrainCost;

	switch (terrain[x][y])
	{
	case CLEARTERRAIN:
		terrainCost = 10;
		break;
	case ROUGHTERRAIN: case WATERTERRAIN:
		terrainCost = 20;
		break;
	default:
		terrainCost = 1;
	}

	if (abs(x - px) == 1 && abs(y - py) == 1)
	{
		gCost = Gcost[px][py] + (DIAGONALCOST * terrainCost);
	}
	else
	{
		gCost = Gcost[px][py] + (ORTHOGONALCOST * terrainCost);
	}

	return gCost;
}

void Enemy::aStarBinaryHeap()
{
	openList[1] = openList[openListSize];
	openListSize--;
	int v = 1;
	do
	{
		int u = v;
		if (2 * u + 1 <= openListSize) //if both children exist
		{
			if (Fcost[openList[u]] >= Fcost[openList[2 * u]])
				v = 2 * u;
			if (Fcost[openList[v]] >= Fcost[openList[2 * u + 1]])
				v = 2 * u + 1;
		}
		else
		{
			if (2 * u <= openListSize) //if only child #1 exists
			{
				if (Fcost[openList[u]] >= Fcost[openList[2 * u]])
					v = 2 * u;
			}
		}

		if (u != v) //if parent's F is > one of its children, swap them
		{
			int temp = openList[u];
			openList[u] = openList[v];
			openList[v] = temp;
		}
		else break; //otherwise, exit loop

	} while (1);
}

void Enemy::aStarBubbleNewF(int m)
{
	while (m != 1) //While item hasn't bubbled to the top (m=1)	
	{
		if (Fcost[openList[m]] <= Fcost[openList[m / 2]])
		{
			int temp = openList[m / 2];
			openList[m / 2] = openList[m];
			openList[m] = temp;
			m = m / 2;
		}
		else
			break;
	}
}

bool Enemy::aStarCutCorner(int a, int b)
{
	//	Don't cut across corners
	bool corner = true;
	if (a == parentXval - 1)
	{
		if (b == parentYval - 1)
		{
			if (terrain[parentXval - 1][parentYval] == BLOCKEDTERRAIN
				|| terrain[parentXval][parentYval - 1] == BLOCKEDTERRAIN)
				corner = false;
		}
		else if (b == parentYval + 1)
		{
			if (terrain[parentXval][parentYval + 1] == BLOCKEDTERRAIN
				|| terrain[parentXval - 1][parentYval] == BLOCKEDTERRAIN)
				corner = false;
		}
	}
	else if (a == parentXval + 1)
	{
		if (b == parentYval - 1)
		{
			if (terrain[parentXval][parentYval - 1] == BLOCKEDTERRAIN
				|| terrain[parentXval + 1][parentYval] == BLOCKEDTERRAIN)
				corner = false;
		}
		else if (b == parentYval + 1)
		{
			if (terrain[parentXval + 1][parentYval] == BLOCKEDTERRAIN
				|| terrain[parentXval][parentYval + 1] == BLOCKEDTERRAIN)
				corner = false;
		}
	}

	return corner;
}

void Enemy::aStarSetMapValues(Map* map, bool swim)
{
	for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x += BLOCK_SIZE)
	{
		for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y += BLOCK_SIZE)
		{
			if (terrain[x / BLOCK_SIZE][y / BLOCK_SIZE] == HASENEMY)
			{
				terrain[x / BLOCK_SIZE][y / BLOCK_SIZE] = BLOCKEDTERRAIN;
			}
			else
			{
				if (swim)
				{
					terrain[x / BLOCK_SIZE][y / BLOCK_SIZE] = map->getTerrain(x, y);
				}
				else
				{
					if (map->getTerrain(x, y) == WATERTERRAIN)
					{
						terrain[x / BLOCK_SIZE][y / BLOCK_SIZE] = BLOCKEDTERRAIN;
					}
					else
					{
						terrain[x / BLOCK_SIZE][y / BLOCK_SIZE] = map->getTerrain(x, y);
					}
				}
			}
		}
	}
}