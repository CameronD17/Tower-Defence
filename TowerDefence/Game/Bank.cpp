#include "Bank.h"

Bank::Bank(void){}
	
void Bank::init(Engine &e, int c)
{
	engine = e;
	credit = c; 
	score = 0;
}

void Bank::draw()
{
	std::stringstream creditText, scoreText;

	// Top area
	creditText << "Credit: " << getCredit();
	engine.graphics.renderText(SIDEBAR_X + BLOCK_SIZE, BORDER_SIZE / 2, creditText.str(), LARGE, 255, 255, 255, "bombardier");

	scoreText << "Score: " << getScore();
	engine.graphics.renderText(SIDEBAR_X + BLOCK_SIZE, 180, scoreText.str(), LARGE, 255, 255, 255, "bombardier");

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

int Bank::getScore()
{
	return score;
}

void Bank::increaseScore(int s)
{
	score += s;
}