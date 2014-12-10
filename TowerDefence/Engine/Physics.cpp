#include "Physics.h"
#include <iostream>
#include <math.h>

Physics::Physics(void)
{
}

Physics::~Physics(void)
{
}

bool Physics::move(Object *o, int dir, int amt)
{
	switch(dir)
	{
		case UP: 			
			if(o->getY() >= BORDER) 
			{
				o->setY(o->getY() - amt);	
				return true;
			}
            break;

        case UPRIGHT: 
			if((o->getY() >= BORDER) && (o->getY() <= (BOARD_WIDTH * BLOCK_SIZE) + BORDER))
			{
				o->setY(o->getY() - amt);
				o->setX(o->getX() + amt);
				return true;
			}
            break;

        case RIGHT:
			if(o->getX() + amt < BOARD_WIDTH * BLOCK_SIZE)
			{
				o->setX(o->getX() + amt);
				return true;
			}
            break;

        case DOWNRIGHT:
			if((o->getY() < (BOARD_HEIGHT * BLOCK_SIZE) + BORDER) && (o->getX() <= (BOARD_WIDTH * BLOCK_SIZE) + BORDER))
			{
				o->setX(o->getX() + amt);
				o->setY(o->getY() + amt);
				return true;
			}
            break;

		case DOWN:	
			if(o->getY() + amt < BOARD_HEIGHT * BLOCK_SIZE)
			{
				o->setY(o->getY() + amt);	
				return true;
			}
            break;

        case DOWNLEFT:
			if((o->getY() < (BOARD_HEIGHT * BLOCK_SIZE) + BORDER) && (o->getX() >= BORDER))
			{
				o->setY(o->getY() + amt);
				o->setX(o->getX() - amt);
				return true;
			}
            break;

        case LEFT:
			if(o->getX() >= BORDER)
			{
				o->setX(o->getX() - amt);
				return true;
			}
            break;

        case UPLEFT:
			if((o->getX() >= BORDER) && (o->getY() >= BORDER))
			{
				o->setX(o->getX() - amt);
				o->setY(o->getY() - amt);
				return true;
			}
            break;

        default:
            break;
	}	
	return false;
}

bool Physics::nonUniformMove(Object *o, int dx, int dy)
{
	o->setX(o->getX() + dx);
	o->setY(o->getY() + dy);
	return true;
}

bool Physics::checkPosition(button b)
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (x < b.rect.x)
	{
		return false;
	}
	else if (x > b.rect.x + b.rect.w)
	{
		return false;
	}
	else if (y < b.rect.y)
	{
		return false;
	}
	else if (y > b.rect.y + b.rect.h)
	{
		return false;
	}

	return true;
}

bool Physics::impreciseCollision(Object *a, Object *b)
{
	float aX = (float)a->getX() + BLOCK_SIZE/2;		// Makes the enemy coordinates into the
	float aY = (float)a->getY() + BLOCK_SIZE/2;		// midpoint rather than the top left corner
	float bX = (float)b->getX();
	float bY = (float)b->getY();
	
	// Distance of a line formula is (root)(((x1-x2)^2) + ((y1-y2)^2). Not using square root, as it's not efficient.
	return (((aX - bX) * (aX - bX)) + ((aY - bY) * (aY - bY))) <= BLOCK_SIZE;
}

bool Physics::checkDistance(Object *tower, Object *enemy)
{
	return (	enemy->getX() > (tower->getX() - (BORDER + BLOCK_SIZE * tower->getType()))			// Within Left boundary
			&&	enemy->getX() < (tower->getX() + BLOCK_SIZE + (BLOCK_SIZE * tower->getType()))		// Within Right boundary
			&&	enemy->getY() > (tower->getY() - (BORDER + BLOCK_SIZE * tower->getType()))			// Within Top boundary
			&&	enemy->getY() < (tower->getY() + BLOCK_SIZE + (BLOCK_SIZE * tower->getType())));	// Within Bottom boundary
}

int Physics::getDirection(Object *tower, Object *enemy)
{
	if(enemy->getY() < tower->getY())			// Enemy is above the tower
	{
		if(enemy->getX() < tower->getX())		// Enemy is also left of the tower
			return UPLEFT;
		else if(enemy->getX() > tower->getX())	// Enemy is also right of the tower
			return UPRIGHT;
		else									// Enemy is directly above the tower
			return UP;
	}	
	else if(enemy->getY() > tower->getY())		// Enemy is below the tower
	{
		if(enemy->getX() < tower->getX())		// Enemy is also left of the tower
			return DOWNLEFT;
		else if(enemy->getX() > tower->getX())	// Enemy is also right of the tower
			return DOWNRIGHT;
		else									// Enemy is directly below the tower
			return DOWN; 
	}
	else if(enemy->getX() < tower->getX())		// Enemy is directly left of the tower
		return LEFT;
	else if(enemy->getX() > tower->getX())		// Enemy is directly right of the tower
		return RIGHT;

	return 0;
}