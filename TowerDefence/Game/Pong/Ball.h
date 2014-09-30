#pragma once
#include "../Constants.h"
#include "../../Engine/Object.h"

class Ball: public Object
{
public:

	Ball();
	~Ball();	
	
	int getDX()const;
	int getDY()const;
	void setDX(int N);
	void setDY(int N);

	void reset();

private:
	int xDirection, yDirection;
};

