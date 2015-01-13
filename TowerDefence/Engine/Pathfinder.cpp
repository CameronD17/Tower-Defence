#include "Pathfinder.h"

Pathfinder::Pathfinder()
{
}

void Pathfinder::initialise(int x, int y, int tX, int tY, Map* m, int i, bool swim)
{
	id = i;
	canSwim = swim;
	findPath(x, y, tX, tY, m);
}

Pathfinder::~Pathfinder()
{

}

int Pathfinder::getID()const
{
	return id;
}

int Pathfinder::getNextX()const
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

int Pathfinder::getNextY()const
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



// *** A-Star Pathfinding functions *** //

bool Pathfinder::findPath(int sX, int sY, int tX, int tY, Map* map)
{
	// Initialise values
	aStarSetMapValues(map, canSwim);

	startX = (sX - BORDER) / BLOCK_SIZE, startY = (sY - BORDER) / BLOCK_SIZE;
	targetX = (tX - BORDER) / BLOCK_SIZE, targetY = (tY - BORDER) / BLOCK_SIZE;

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
				if (x > -1 && y > -1 && x < BOARD_WIDTH && y < BOARD_HEIGHT	// If the tile is not off the map
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

void Pathfinder::aStarCalcPath()
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

int Pathfinder::aStarGetGCost(int x, int y, int px, int py)
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

void Pathfinder::aStarBinaryHeap()
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

void Pathfinder::aStarBubbleNewF(int m)
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

bool Pathfinder::aStarCutCorner(int a, int b)
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

void Pathfinder::aStarSetMapValues(Map* map, bool swim)
{
	for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x += BLOCK_SIZE)
	{
		for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y += BLOCK_SIZE)
		{
			if (terrain[x / BLOCK_SIZE][y / BLOCK_SIZE] == HASENEMY || terrain[x / BLOCK_SIZE][y / BLOCK_SIZE] == HASTOWER)
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