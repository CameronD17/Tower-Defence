#pragma once
#include "../Engine/Constants.h"
#include "../Engine/Engine.h"
#include "../Engine/Object.h"
#include "Tower.h"

class Cursor: public Object
{
private:
	Engine engine;
	int action;
	tStats towerStats;

public:
	Cursor();
	Cursor(int x, int y); 
	~Cursor();	
	
	int getAction()const;
	void setAction(int t); 
	
	tStats getStats()const;

	void init(Engine &e);
	void draw();
	void reset();
};

