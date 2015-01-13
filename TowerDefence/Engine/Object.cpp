#include "Object.h"

Object::Object()
{
	id = 0;
    coordinate.xPos = 0;
	coordinate.yPos = 0;
	deleted = false;
	texture = NULL;
}

Object::Object(int x, int y, int i)
{
	id = i;
	coordinate.xPos = x;
	coordinate.yPos = y;
	deleted = false;
	texture = NULL;
}

Object::~Object()
{

}

int Object::getX()const
{
	return coordinate.xPos;
}

int Object::getY()const
{
	return coordinate.yPos;
}

int Object::getID()const
{
	return id;
}

coordinates Object::getCoordinates()const
{
	return coordinate;
}

ResourceManager Object::getResources()const
{
	return resource;
}

SDL_Texture* Object::getTexture()const
{
	return texture;
}

bool Object::isDeleted()const
{
	return deleted;
}

void Object::setX(int N)
{
	coordinate.xPos = N;
}

void Object::setY(int N)
{
	coordinate.yPos = N;
}

void Object::setID(int N)
{
	id = N;
}

void Object::setCoordinates(int x, int y)
{
	coordinate.xPos = x;
	coordinate.yPos = y;
}

void Object::setResources(ResourceManager r)
{
	resource = r;
}

void Object::setTexture(SDL_Texture* t)
{
	texture = t;
}

void Object::setDeleted(bool d)
{
	this->deleted = d;
}
