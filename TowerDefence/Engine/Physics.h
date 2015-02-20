#pragma once
#include "Object.h"
#include "../Engine/Constants.h"
#include <vector>

class Physics
{

public:	
	Physics(void);
	~Physics(void);

	bool move(Object *o, int dir, int amt);
	bool nonUniformMove(Object* o, float xDir, float yDir);
	bool impreciseCollision(Object *enemy, Object *bullet);
};
