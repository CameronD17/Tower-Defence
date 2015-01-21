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

	(*m).setTerrain(x - BORDER_SIZE, y - BORDER_SIZE, HAS_ENEMY);
	(*m).setEnemy(x - BORDER_SIZE, y - BORDER_SIZE, stats.id);
}

bool Enemy::reduceHealth(int i, Map* m)
{
	stats.currentHealth -= i;
	if (stats.currentHealth <= 0)
	{
		setDeleted(true);
		releaseAllMyTiles(m);
		return true;
	}
	return false;
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

int Enemy::getSpeed()const
{
	return stats.speed;
}

int Enemy::getNextMove()const
{
	return  astar.getNextMove();
}

eStats Enemy::getStats()const
{
	return stats;
}

Pathfinder Enemy::getPath()const
{
	return astar;
}

bool Enemy::canWalk(Map* map)
{
	// Check if you're there first to avoid unnecessary calculations
	if (stats.targetX == getX() && stats.targetY == getY())
	{
		setDeleted(true);
		releaseAllMyTiles(map);
		return false;
	}

	// Hey, you've committed to moving into the next square. Trust your instincts.
	if (stepsTaken > 0 && stepsTaken < stepsPerSquare)
	{
		stepsTaken++;
		return true;
	}

	// Ooh, time to try and move into a new square. Just make sure you haven't reached the end of your path though. That would make everyone look silly.
	else if (getNextMove() != 0)
	{
		// Okay, we haven't reached the base yet. Pop-off the path coordinates of the current tile to access the next tile
		if (stepsTaken == stepsPerSquare)
		{
			astar.popBack();			
		}

		// Let's make sure that the next square isn't occupied.
		if (map->walkable(astar.getNextX(), astar.getNextY(), stats.id))
		{
			// GET IN! The next tile is available. Let's move!
			releaseAllMyTiles(map);
			lockThisTile(map);
			lockNextTile(map);
			stepsTaken = 1;	
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

	// You've reached the end of your path!
	return false;	
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
				map->setTerrain(x, y, CLEAR_TERRAIN);
				map->setEnemy(x, y, 0);
			}
		}
	}
}

void Enemy::lockThisTile(Map* map)
{
	int thisX = getX() - (getX() % BLOCK_SIZE) - BORDER_SIZE;
	int thisY = getY() - (getY() % BLOCK_SIZE) - BORDER_SIZE;

	if (map->walkable(thisX, thisY, stats.id))
	{
		map->setTerrain(thisX, thisY, HAS_ENEMY);
		map->setEnemy(thisX, thisY, stats.id);
	}
}

void Enemy::lockNextTile(Map* map)
{
	int thisX = getX() - (getX() % BLOCK_SIZE) - BORDER_SIZE;
	int thisY = getY() - (getY() % BLOCK_SIZE) - BORDER_SIZE;
	int otherX = thisX;
	int otherY = thisY;	
	int nextMove = astar.getNextMove();

	switch (nextMove)
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
	case UP_LEFT:
		thisX -= BLOCK_SIZE;
		thisY -= BLOCK_SIZE;
		break;
	case UP_RIGHT:
		thisX += BLOCK_SIZE;
		thisY -= BLOCK_SIZE;
		break;
	case DOWN_RIGHT:
		thisX += BLOCK_SIZE;
		thisY += BLOCK_SIZE;
		break;
	case DOWN_LEFT:
		thisX -= BLOCK_SIZE;
		thisY += BLOCK_SIZE;
		break;
	default:
		break;
	}

	if (map->walkable(thisX, thisY, stats.id))
	{
		map->setTerrain(thisX, thisY, HAS_ENEMY);
		map->setEnemy(thisX, thisY, stats.id);
	}

	if (nextMove == UP_RIGHT || nextMove == UP_LEFT || nextMove == DOWN_RIGHT || nextMove == DOWN_LEFT)
	{
		if (map->walkable(otherX, thisY, stats.id))
		{
			map->setTerrain(otherX, thisY, HAS_ENEMY);
			map->setEnemy(otherX, thisY, stats.id);
		}

		if (map->walkable(thisX, otherY, stats.id))
		{
			map->setTerrain(thisX, otherY, HAS_ENEMY);
			map->setEnemy(thisX, otherY, stats.id);
		}
	}
}