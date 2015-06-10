#pragma once
#include "boost/smart_ptr.hpp"
#include "../Engine/Object.h"
#include "Map.h"
#include"Enemy.h"

class Bullet: public Object 
{
private:
	int startX, startY, range;
	float speed, dX, dY;
	bool hitTarget;

public:
	Bullet();
	Bullet(int x, int y, int range, int tX, int tY);
	~Bullet();

	float getDX();
	float getDY();
	bool hit();
	
	void update(boost::shared_ptr<Enemy> e);
};
