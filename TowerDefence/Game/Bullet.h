#pragma once
#include "Constants.h"
#include "../Engine/Object.h"

class Bullet: public Object
{
public:

	Bullet();
	Bullet(int x, int y, int dir);
	~Bullet();
	
	int direction, r, g, b;
private:
	
};

