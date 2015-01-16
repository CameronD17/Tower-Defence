#pragma once
#include "../Engine/Constants.h"
#include "../Engine/Object.h"

class Cursor: public Object
{
public:
	Cursor();
	Cursor(int x, int y); 
	~Cursor();	
	int r, g, b;
	int currentSelection;

	int getAction()const;
	void setAction(int t);

	void init(ResourceManager &rm);
	void reset();
};

