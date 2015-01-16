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
	enemy = e;
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

bool Bullet::expired()
{
	// Bullets can't travel further than the parent tower range, nor continue if they hit their target.
	if ((abs(startX - getX()) > range || abs(startY - getY()) > range))
	{
		setDeleted(true);
		return true;
	}

	if (!(*enemy).isDeleted())
	{
		if (getX() > (*enemy).getX() && getX() < ((*enemy).getX() + BLOCK_SIZE) && getY() > (*enemy).getY() && getY() < ((*enemy).getY() + BLOCK_SIZE))
		{
			hit = true;
			setDeleted(true);
			return true;
		}
	}
	
	return false;
}