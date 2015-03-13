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

Enemy::Enemy(Engine &e, int x, int y, int t, int tX, int tY, int l, Map &m, int i)
{
	engine = e;
	setResources(engine.resources);
	setID(i);
	stats.id = getID();
	initialise(l, x, y, tX, tY, t, m);	
	astar.initialise(x, y, tX, tY, m, i, false);
}

Enemy::~Enemy(){}

void Enemy::initialise(int level, int x, int y, int tX, int tY, int t, Map &m)
{
	setX(x);
	setY(y);

	stats.targetX = tX;
	stats.targetY = tY;

	stats.currentHealth = stats.maxHealth = (float)(level * HEALTH_MULTIPLIER * 10);
	stats.value = level * VALUE_MULTIPLIER;
	stats.bounty = level * BOUNTY_MULTIPLIER;
	stats.type = t;
	
	switch (t)
	{
	case BASIC_SOLDIER: 
	case SUPER_SOLDIER: 
	case GHOST_SOLDIER:
		stats.canSwim = true;
		break;
	default:
		stats.canSwim = false;
		break;
	}

	switch (t)
	{
	case GHOST_SOLDIER:
	case SUPER_TANK:
		stats.speed = 1;
	case SUPER_SOLDIER:
	default:
		stats.speed = 2;
		break;
	case BASIC_SOLDIER:
	case BASIC_TANK:
		stats.speed = 3;
		break;
	case JEEP:
		stats.speed = 4;
		break;
	case MOTORBIKE:
		stats.speed = 6;
		break;
	}

	stepsTaken = 0;
	stepsPerSquare = (int)(TILE_SIZE / stats.speed);

	m.setEnemy(x - BORDER_SIZE, y - BORDER_SIZE, stats.id);
}

void Enemy::draw()
{
	switch (stats.type)
	{
	case BASIC_SOLDIER: default:
		engine.graphics.drawRectangle(getX() + 4, getY() + 4, TILE_SIZE - 8, TILE_SIZE - 8, 237, 28, 36);
		break; 
	case MOTORBIKE:
		engine.graphics.drawRectangle(getX(), getY(), TILE_SIZE, TILE_SIZE, 255, 0, 0);
		break;
	case JEEP:
		engine.graphics.drawRectangle(getX(), getY(), TILE_SIZE, TILE_SIZE, 255, 165, 0);
		break;
	case BASIC_TANK:
		engine.graphics.drawRectangle(getX(), getY(), TILE_SIZE, TILE_SIZE, 124, 252, 0);
		break;
	case SUPER_SOLDIER:
		engine.graphics.drawRectangle(getX() + 4, getY() + 4, TILE_SIZE - 8, TILE_SIZE - 8, 128, 0, 128);
		break;
	case SUPER_TANK:
		engine.graphics.drawRectangle(getX(), getY(), TILE_SIZE, TILE_SIZE, 0, 100, 0);
		break;
	case GHOST_SOLDIER:
		engine.graphics.drawRectangle(getX() + 4, getY() + 4, TILE_SIZE - 8, TILE_SIZE - 8, 139, 137, 137);
		break;
	}

	engine.graphics.drawRectangle(getX(), getY(), (int)((stats.currentHealth / stats.maxHealth) * TILE_SIZE), 2, 0, 255, 0);
}

bool Enemy::reduceHealth(int i, Map &m, Bank &bank)
{
	stats.currentHealth -= i;
	if (stats.currentHealth <= 0)
	{
		destroy();
		releaseTiles(m);
		bank.increaseScore(stats.bounty);
		return true;
	}
	return false;
}

void Enemy::updateTarget(int tX, int tY, Map &m)
{
	setX(getX() - getX() % TILE_SIZE);
	setY(getY() - getY() % TILE_SIZE);
	stats.targetX = tX, stats.targetY = tY;

	stepsTaken = 0;

	astar.findPath(getX(), getY(), tX, tY, m);
}

void Enemy::updatePath(Map &m, int x, int y)
{
	if (x == -1 && y == -1)
	{
		setX(getX() - getX() % TILE_SIZE);
		setY(getY() - getY() % TILE_SIZE);
		stepsTaken = 0;
		astar.findPath(getX(), getY(), stats.targetX, stats.targetY, m);
	}
	else
	{
		for (unsigned i = astar.getPathSize(); i > 0; i--)
		{
			int pathX = (astar.getXAt(i) * TILE_SIZE) + BORDER_SIZE;
			int pathY = (astar.getYAt(i) * TILE_SIZE) + BORDER_SIZE;

			if (pathX == x && pathY == y)
			{
				setX(getX() - getX() % TILE_SIZE);
				setY(getY() - getY() % TILE_SIZE);
				stepsTaken = 0;
				astar.findPath(getX(), getY(), stats.targetX, stats.targetY, m);
				break;
			}
		}
	}
}

void Enemy::move(Map &map)
{
	// Check if you've arrived first to avoid unnecessary calculations
	if (stats.targetX == getX() && stats.targetY == getY())
	{
		destroy();
		releaseTiles(map);
		return;
	}

	// Hey, you've committed to moving into the next square. Trust your instincts.
	if (stepsTaken > 0 && stepsTaken < stepsPerSquare)
	{
		stepsTaken++;
		engine.physics.move(this, astar.getNextMove(), stats.speed);
	}

	// Ooh, time to try and move into a new square. Just make sure you haven't reached the end of your path though. That would make everyone look silly.
	else if (astar.getNextMove() != 0)
	{
		// Okay, we haven't reached the base yet. Pop-off the path coordinates of the current tile to access the next tile
		if (stepsTaken == stepsPerSquare)	astar.popBack();

		// Let's make sure that the next square isn't occupied.
		if (map.walkable(astar.getNextX(), astar.getNextY(), stats.id, astar.getNextMove()))
		{
			// GET IN! The next tile is available. Let's move!
			releaseTiles(map);
			lockTiles(map);
			stepsTaken = 1;	
			engine.physics.move(this, astar.getNextMove(), stats.speed);
		}
		
		// Well shit, we can't move. Is there some other way we can get around to the base?
		// Let's hope so. We'll update the path with the new values and try again next move.
		else
		{
			releaseTiles(map);
		//	updatePath(map);
		}
	}
}

void Enemy::releaseTiles(Map &map)
{
	for (int x = getX() - (2 * TILE_SIZE); (x < getX() + (2 * TILE_SIZE)) && (x < BOARD_WIDTH); x += TILE_SIZE)
	{
		for (int y = getY() - (2 * TILE_SIZE); (y < getY() + (2 * TILE_SIZE)) && (y < BOARD_HEIGHT); y += TILE_SIZE)
		{
			if (map.getEnemy(x, y) == stats.id)
			{
				map.setEnemy(x, y, NO_ENEMY);
			}
		}
	}
}

void Enemy::lockTiles(Map &map)
{
	int thisX = getX() - (getX() % TILE_SIZE) - BORDER_SIZE;
	int thisY = getY() - (getY() % TILE_SIZE) - BORDER_SIZE;
	int otherX = thisX;
	int otherY = thisY;	
	int nextMove = astar.getNextMove();

	// Lock the current tile
	if (map.walkable(thisX, thisY, stats.id))
	{
		map.setEnemy(thisX, thisY, stats.id);
	}

	switch (nextMove)
	{
	case UP: 
		thisY -= TILE_SIZE;
		break;
	case RIGHT:
		thisX += TILE_SIZE;
		break;
	case DOWN:
		thisY += TILE_SIZE;
		break;
	case LEFT:
		thisX -= TILE_SIZE;
		break;
	case UP_LEFT:
		thisX -= TILE_SIZE;
		thisY -= TILE_SIZE;
		break;
	case UP_RIGHT:
		thisX += TILE_SIZE;
		thisY -= TILE_SIZE;
		break;
	case DOWN_RIGHT:
		thisX += TILE_SIZE;
		thisY += TILE_SIZE;
		break;
	case DOWN_LEFT:
		thisX -= TILE_SIZE;
		thisY += TILE_SIZE;
		break;
	default:
		break; 
	}
	
	// Lock the next tile
	if (map.walkable(thisX, thisY, stats.id))
	{
		map.setEnemy(thisX, thisY, stats.id);
	}

	// Lock periphery tiles if required
	if (nextMove == UP_RIGHT || nextMove == UP_LEFT || nextMove == DOWN_RIGHT || nextMove == DOWN_LEFT)
	{
		if (map.walkable(otherX, thisY, stats.id))
		{
			map.setEnemy(otherX, thisY, stats.id);
		}

		if (map.walkable(thisX, otherY, stats.id))
		{
			map.setEnemy(thisX, otherY, stats.id);
		}
	}
}