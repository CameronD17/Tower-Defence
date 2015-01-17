#include "Bullet.h"

Bullet::Bullet() 
{
    setX(0);
	setY(0);
}

Bullet::~Bullet()
{

}

Bullet::Bullet(int x, int y, int r, Enemy* e)
{
	setX(x);
	setY(y);
	range = r;
	startX = x;
	startY = y;
	targetX = (*e).getX();
	targetY = (*e).getY();
	speed = 3;
	dX = ((targetX - x) / BLOCK_SIZE) * (float)speed;
	dY = ((targetY - y) / BLOCK_SIZE) * (float)speed;
	hit = false;
}

float Bullet::getDX()
{
	return dX;
}

float Bullet::getDY()
{
	return dY;
}

bool Bullet::hasHit()
{
	return hit;
}

bool Bullet::update(Enemy* e)
{
	// Bullets can't travel further than the parent tower range, nor continue if they hit their target.
	if ((abs(startX - getX()) > range || abs(startY - getY()) > range))
	{
		setDeleted(true);
		return true;
	}
	else if (e != NULL)
	{
		if (getX() > e->getX() && getX() < (e->getX() + BLOCK_SIZE) && getY() > e->getY() && getY() < (e->getY() + BLOCK_SIZE))
		{
			hit = true;
			setDeleted(true);
			return true;
		}
	}
	else
	{
		setDeleted(true);
		return true;
	}
	
	return false;
}