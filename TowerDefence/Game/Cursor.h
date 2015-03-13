#pragma once
#include "../Engine/Constants.h"
#include "../Engine/Engine.h"
#include "../Engine/Object.h"

class Cursor: public Object
{
private:
	Engine engine;
	int action;

public:
	Cursor();
	Cursor(int x, int y); 
	~Cursor();	
	
	int getAction()const;
	int getCost()const;
	void setAction(int t); 
	
	void init(Engine &e);
	void draw();
	void reset();
};

