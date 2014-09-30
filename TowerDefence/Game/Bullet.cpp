#include "Bullet.h"

Bullet::Bullet() 
{
    setX(0);
	setY(0);
    r = b = g = 0xFF;
	setType(0);
	direction = 0;
}

Bullet::~Bullet()
{

}

Bullet::Bullet(int x, int y, int dir) 
{
    setX(x);
    setY(y);
    r = b = g = 0xFF;
	setType(0);
	direction = dir;
}
