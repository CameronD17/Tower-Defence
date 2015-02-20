#pragma once
#include "../Engine/Constants.h"
#include "../Engine/Engine.h"
#include "Tower.h"
#include "Cursor.h"
 
class Bank 
{
private:		
	int credit;
	int score;
	Engine engine;

public:	
	Bank(void);
		
	int getCredit();
	void increaseCredit(int c);
	void decreaseCredit(int c);
	void setCredit(int c);

	int getScore();
	void increaseScore(int s);

	void init(Engine &e, int c);
	void draw();
};
