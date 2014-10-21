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
}

Tower::Tower(int x, int y, int t) 
{
	setX(x);
	setY(y);
	setType(t);
	setStats(t);
};

Tower::~Tower()
{

}

void Tower::setStats(int t)
{
	level = 0;
	hits = 0;
	kills = 0;

	switch (t)
	{

	// Machine Gun
	case 1:
		cost = 50;
		damage = 4;
		range = 7;
		reload = 20;
		break;

	// Light Cannon
	case 2:
		cost = 75;
		damage = 10;
		range = 7;
		reload = 100;
		break;

	// Default
	case 0: default:
		cost = 0;
		damage = 0;
		range = 0;
		reload = 0;
		break;
	}
}

void Tower::fire(long dir)
{
	//Bullet * b = new Bullet((getX() + (BLOCK_SIZE/2)), (getY() + (BLOCK_SIZE/2)), dir);
	//bullets.push_back(b);
}


