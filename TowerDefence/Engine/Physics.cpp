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
			if(o->getY() >= BORDER_SIZE) 
			{
				o->setY(o->getY() - amt);	
				return true;
			}
            break;

        case UP_RIGHT: 
			if((o->getY() >= BORDER_SIZE) && (o->getY() <= (BOARD_WIDTH) + BORDER_SIZE))
			{
				o->setY(o->getY() - amt);
				o->setX(o->getX() + amt);
				return true;
			}
            break;

        case RIGHT:
			if(o->getX() + amt <= BOARD_WIDTH)
			{
				o->setX(o->getX() + amt);
				return true;
			}
            break;

        case DOWN_RIGHT:
			if((o->getY() <= (BOARD_HEIGHT) + BORDER_SIZE) && (o->getX() <= (BOARD_WIDTH) + BORDER_SIZE))
			{
				o->setX(o->getX() + amt);
				o->setY(o->getY() + amt);
				return true;
			}
            break;

		case DOWN:	
			if(o->getY() + amt <= BOARD_HEIGHT)
			{
				o->setY(o->getY() + amt);	
				return true;
			}
            break;

        case DOWN_LEFT:
			if((o->getY() <= (BOARD_HEIGHT) + BORDER_SIZE) && (o->getX() >= BORDER_SIZE))
			{
				o->setY(o->getY() + amt);
				o->setX(o->getX() - amt);
				return true;
			}
            break;

        case LEFT:
			if(o->getX() >= BORDER_SIZE)
			{
				o->setX(o->getX() - amt);
				return true;
			}
            break;

        case UP_LEFT:
			if((o->getX() >= BORDER_SIZE) && (o->getY() >= BORDER_SIZE))
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

bool Physics::nonUniformMove(Object *o, float dx, float dy)
{
	o->setX(o->getX() + (int)dx);
	o->setY(o->getY() + (int)dy);
	return true;
}

//bool Physics::checkPosition(button b)
//{
//	int x, y;
//	SDL_GetMouseState(&x, &y);
//
//	if (x < b.rect.x)
//	{
//		return false;
//	}
//	else if (x > b.rect.x + b.rect.w)
//	{
//		return false;
//	}
//	else if (y < b.rect.y)
//	{
//		return false;
//	}
//	else if (y > b.rect.y + b.rect.h)
//	{
//		return false;
//	}
//
//	return true;
//}

bool Physics::impreciseCollision(Object *a, Object *b)
{
	float aX = (float)a->getX() + TILE_SIZE/2;		// Makes the enemy coordinates into the
	float aY = (float)a->getY() + TILE_SIZE/2;		// midpoint rather than the top left corner
	float bX = (float)b->getX();
	float bY = (float)b->getY();
	
	// Distance of a line formula is (root)(((x1-x2)^2) + ((y1-y2)^2). Not using square root, as it's not efficient.
	return (((aX - bX) * (aX - bX)) + ((aY - bY) * (aY - bY))) <= TILE_SIZE;
}