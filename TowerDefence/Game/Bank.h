#pragma once

#include "../Engine/Constants.h"
#include "Tower.h"
#include "Cursor.h"
using namespace std;

class Bank 
{

private:		
	int credit;
		
public:	
	Bank(void);
		
	int getCredit();
	void increaseCredit(int c);
	void decreaseCredit(int c);
	void setCredit(int c);

	void init(int c);
};
