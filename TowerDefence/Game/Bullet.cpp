#include "Bullet.h"

Bullet::Bullet() 
{
    setX(0);
	setY(0);
	setType(0);
}

Bullet::~Bullet()
{

}

Bullet::Bullet(int x, int y, int tX, int tY, int r)
{
	setX(x);
	setY(y);
	range = r;
	startX = x;
	startY = y;
	targetX = tX;
	targetY = tY;
	speed = 4;
	dX = ((tX - x) / BLOCK_SIZE) * speed;
	dY = ((tY - y) / BLOCK_SIZE) * speed;
}

bool Bullet::hitTarget()
{
	// Bullets can't travel further than the parent tower range, nor continue if they hit their target.
	if ((abs(startX - getX()) > range || abs(startY - getY()) > range) || (getX() == targetX && getY() == targetY))
	{
		setDeleted(true);
		return true;
	}

	return false;
}