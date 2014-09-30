#include "Object.h"

Object::Object()
{
    xPos = 0;
    yPos = 0;
	deleted = false;
	texture = NULL;
}

Object::Object(int x, int y)
{
    xPos = x;
    yPos = y;
	deleted = false;
	texture = NULL;
}

Object::~Object()
{

}

int Object::getX()const
{
	return xPos;
}

int Object::getY()const
{
	return yPos;
}

int Object::getType()const
{
	return type;
}

bool Object::isDeleted()const
{
	return deleted;
}

void Object::setX(int N)
{
	this->xPos = N;
}

void Object::setY(int N)
{
	this->yPos = N;
}

void Object::setType(int N)
{
	this->type = N;
}

void Object::setDeleted(bool d)
{
	this->deleted = d;
}
