#include "Ball.h"

Ball::Ball() 
{
    setX(SCREEN_WIDTH / 2);
	setY(SCREEN_HEIGHT / 2);
	setDX(0);
	setDY(0);
}

Ball::~Ball()
{

}

int Ball::getDX()const
{
	return xDirection;
}

int Ball::getDY()const
{
	return yDirection;
}

void Ball::setDX(int N)
{
	this->xDirection = N;
}

void Ball::setDY(int N)
{
	this->yDirection = N;
}

void Ball::reset()
{
	setX(SCREEN_WIDTH / 2);
	setY(SCREEN_HEIGHT / 2);
	setDX(0);
	setDY(0);
}
