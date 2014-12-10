#pragma once
#include "../Engine/Object.h"
#include "Enemy.h"
#include "Map.h"


class Bullet: public Object 
{

public:

	Bullet();
	Bullet(int x, int y, int tX, int tY, int range);
	~Bullet();

	int startX, startY, targetX, targetY, range, speed;
	float angle, dX, dY;
	bool hitTarget();
private:
};
