#include "Bank.h"

Bank::Bank(void){}
	
void Bank::init(int c)
{
	credit = c; 
}

int Bank::getCredit()
{
	return credit;
}

void Bank::increaseCredit(int c)
{
	credit += c;
}

void Bank::decreaseCredit(int c)
{
	credit -= c;
}

void Bank::setCredit(int c)
{
	credit = c;
}