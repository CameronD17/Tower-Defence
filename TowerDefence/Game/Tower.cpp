#include "Tower.h"
#include <iostream>

Tower::Tower() 
{
	setX(0);
    setY(0);
	setType(0);
	damage = 10;
}

Tower::Tower(int x, int y) 
{
	setX(x);
	setY(y);
	setType(0);
	damage = 10;
};

Tower::Tower(int x, int y, int t, SDL_Texture * tex) 
{
	level = 0;
	killCount = 0;
	setX(x);
	setY(y);
	setType(t);
	damage = 10*t;
	reloadRate = t*BULLET_TICK_LENGTH;
	texture = tex;
	getTargets();
};

Tower::~Tower()
{

}

void Tower::fire(int dir)
{
	Bullet * b = new Bullet((getX() + (BLOCK_SIZE/2)), (getY() + (BLOCK_SIZE/2)), dir);
	bullets.push_back(b);
}

void Tower::getTargets()
{
	switch(getType())
	{
	case 1:
		antiGround = true;
		antiAir = true;
		break;
	case 2:
		antiAir = true;
		antiGround = false;
		break;
	case 3:
		antiAir = false;
		antiGround = true;
		break;
	default:
		antiAir = true;
		antiGround = true;
		break;
	}
}

void Tower::levelUp()
{
	killCount++;
	if(killCount % 5 == 0)
	{
		level++;
		if(level < 4)
		{
			damage+=10;
		}
		else if (level < 6)
		{
			reloadRate /= 2;
		}
	}
}