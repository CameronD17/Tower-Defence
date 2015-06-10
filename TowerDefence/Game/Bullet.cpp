#include "Bullet.h"

Bullet::Bullet() 
{
    setX(0);
	setY(0);
}

Bullet::~Bullet(){}

Bullet::Bullet(int x, int y, int r, int tX, int tY)
{
	setX(x);
	setY(y);
	range = r;
	startX = x;
	startY = y;
	speed = 3.0f;
	dX = ((tX - x) / TILE_SIZE) * speed;
	dY = ((tY - y) / TILE_SIZE) * speed;
	hitTarget = false;
}

float Bullet::getDX()
{
	return dX;
}

float Bullet::getDY()
{
	return dY;
}

bool Bullet::hit()
{
	return hitTarget;
}

void Bullet::update(boost::shared_ptr<Enemy> e)
{
	// Bullets can't travel further than the parent tower range, nor continue if they hit their target.
	if ((abs(startX - getX()) > range || abs(startY - getY()) > range))
	{
		destroy();
	}
	else if (e != NULL)
	{
		if (getX() > e->getX() && getX() < (e->getX() + TILE_SIZE) && getY() > e->getY() && getY() < e->getY() + TILE_SIZE)
		{
			hitTarget = true;
			destroy();
		}
	}
	else
	{
		destroy();
	}
}