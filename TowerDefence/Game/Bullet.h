#pragma once
#include "../Engine/Object.h"
#include "Enemy.h"
#include "Map.h"


class Bullet: public Object 
{

private:

	int startX, startY, targetX, targetY, range, speed;
	float angle, dX, dY;
	bool hit;

public:

	Bullet();
	Bullet(int x, int y, int range, Enemy* e);
	~Bullet();

	float getDX();
	float getDY();
	bool hasHit();
	
	bool update(Enemy* e);
};
