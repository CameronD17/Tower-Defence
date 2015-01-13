#include "Enemy.h"

Enemy::Enemy()
{
	setID(0);
    setX(0);
    setY(0);

	stats.targetX = 0;
	stats.targetY = 0;

	stats.currentHealth = 30;
	stats.value = 150;
	stats.bounty = 20;
	stepsTaken = 0;
}

Enemy::Enemy(ResourceManager &rm, int x, int y, int t, int tX, int tY, int l, Map* m, int i)
{
	setResources(rm);
	setID(i);
	stats.id = getID();
	initialise(l, x, y, tX, tY, t, m);	
	astar.initialise(x, y, tX, tY, m, i, false);
}

Enemy::~Enemy()
{

}

void Enemy::initialise(int level, int x, int y, int tX, int tY, int t, Map* m)
{
	setX(x);
	setY(y);

	stats.targetX = tX;
	stats.targetY = tY;

	stats.leftBase = false;
	stats.currentHealth = stats.maxHealth = level * HEALTH_MULTIPLIER;
	stats.value = level * VALUE_MULTIPLIER;
	stats.bounty = level * BOUNTY_MULTIPLIER;
	stats.canSwim = false;		// Change to check type
	stats.speed = (rand() % 4) + 1;				// Change to check type

	stepsTaken = 0;
	stepsPerSquare = (int)(BLOCK_SIZE / stats.speed);

	(*m).setTerrain(x - BORDER, y - BORDER, HASENEMY);
	(*m).setEnemy(x - BORDER, y - BORDER, stats.id);
}

void Enemy::updateTarget(int tX, int tY, Map* m)
{
	setX(getX() - getX() % BLOCK_SIZE);
	setY(getY() - getY() % BLOCK_SIZE);
	stats.targetX = tX, stats.targetY = tY;

	stepsTaken = 0;

	astar.findPath(getX(), getY(), tX, tY, m);
}

void Enemy::updatePath(Map* m)
{
	setX(getX() - getX() % BLOCK_SIZE);
	setY(getY() - getY() % BLOCK_SIZE);

	stepsTaken = 0;

	astar.findPath(getX(), getY(), stats.targetX, stats.targetY, m);
}

void Enemy::getSprites()
{
	sprites.push_back(getResources().getImage("Game/Images/Sprites/enemyUp.png"));
	sprites.push_back(getResources().getImage("Game/Images/Sprites/enemyDown.png"));
	sprites.push_back(getResources().getImage("Game/Images/Sprites/enemyLeft.png"));
	sprites.push_back(getResources().getImage("Game/Images/Sprites/enemyRight.png"));
}

int Enemy::getSpeed()const
{
	return stats.speed;
}

eStats Enemy::getStats()const
{
	return stats;
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
			astar.pathToFollow.pop_back();
			astar.xCoordinates.pop_back();
			astar.yCoordinates.pop_back();
		}

		// Let's make sure that the next square isn't occupied.
		if (map->walkable(astar.getNextX(), astar.getNextY(), stats.id))
		{
			// GET IN! The next tile is available. Let's move!
			moveIntoNewTile(map);
			return true;
		}
		
		// Well shit, we can't move. Is there some other way we can get around to the base?
		// Let's hope so. We'll update the path with the new values and try again next move.
		else
		{
			astar.findPath(getX(), getY(), stats.targetX, stats.targetY, map);
			return false;
		}
	}

	// You've reached the end of your path!
	return false;	
}

int Enemy::nextMove()
{
	int nextDir = (astar.pathToFollow.size() > 0) ?  astar.pathToFollow.back() : 0;

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
			if (map->getEnemy(x, y) == stats.id)
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

	if (map->walkable(thisX, thisY, stats.id))
	{
		map->setTerrain(thisX, thisY, HASENEMY);
		map->setEnemy(thisX, thisY, stats.id);
	}
}

void Enemy::lockNextTile(Map* map)
{
	int thisX = getX() - (getX() % BLOCK_SIZE) - BORDER;
	int thisY = getY() - (getY() % BLOCK_SIZE) - BORDER;
	int otherX = thisX;
	int otherY = thisY;	

	switch (astar.pathToFollow.back())
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

	if (map->walkable(thisX, thisY, stats.id))
	{
		map->setTerrain(thisX, thisY, HASENEMY);
		map->setEnemy(thisX, thisY, stats.id);
	}

	if (astar.pathToFollow.back() == UPRIGHT || astar.pathToFollow.back() == UPLEFT || astar.pathToFollow.back() == DOWNRIGHT || astar.pathToFollow.back() == DOWNLEFT)
	{
		if (map->walkable(otherX, thisY, stats.id))
		{
			map->setTerrain(otherX, thisY, HASENEMY);
			map->setEnemy(otherX, thisY, stats.id);
		}

		if (map->walkable(thisX, otherY, stats.id))
		{
			map->setTerrain(thisX, otherY, HASENEMY);
			map->setEnemy(thisX, otherY, stats.id);
		}
	}
}

bool Enemy::checkWalkabilityOfNextTile(Map* map)
{
	bool isFree = false;

	switch (astar.pathToFollow.back())
	{
	case UP: case DOWN: case LEFT: case RIGHT:
		isFree = (map->walkable(astar.getNextX(), astar.getNextY(), stats.id));
		break;

	case UPRIGHT: 
		isFree = (map->walkable(astar.getNextX(), astar.getNextY(), stats.id)
			&& map->walkable(astar.getNextX(), astar.getNextY() - BLOCK_SIZE, stats.id)
			&& map->walkable(astar.getNextX() + BLOCK_SIZE, astar.getNextY(), stats.id));
		break;

	case UPLEFT:
		isFree = (map->walkable(astar.getNextX(), astar.getNextY(), stats.id)
			&& map->walkable(astar.getNextX(), astar.getNextY() - BLOCK_SIZE, stats.id)
			&& map->walkable(astar.getNextX() - BLOCK_SIZE, astar.getNextY(), stats.id));
		break;

	case DOWNRIGHT:
		isFree = (map->walkable(astar.getNextX(), astar.getNextY(), stats.id)
			&& map->walkable(astar.getNextX(), astar.getNextY() + BLOCK_SIZE, stats.id)
			&& map->walkable(astar.getNextX() + BLOCK_SIZE, astar.getNextY(), stats.id));
		break;

	case DOWNLEFT:
		isFree = (map->walkable(astar.getNextX(), astar.getNextY(), stats.id)
			&& map->walkable(astar.getNextX(), astar.getNextY() + BLOCK_SIZE, stats.id)
			&& map->walkable(astar.getNextX() - BLOCK_SIZE, astar.getNextY(), stats.id));
		break;

	default:
		break;
	}

	return isFree;
}

bool Enemy::reachTarget(Map* map)
{
	if (stats.targetX == getX() && stats.targetY == getY())
	{
		setDeleted(true);
		releaseAllMyTiles(map);
		return true;
	}
	return false;
}