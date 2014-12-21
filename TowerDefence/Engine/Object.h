#pragma once
#include "../Engine/ResourceManager.h"

/*-------------------------------------------------------- *
 * OBJECT
 * Strictly speaking, this probably shouldn't be part of the
 * engine, but I needed something for the Physics class to 
 * recognise when I passed things to it
 *-------------------------------------------------------- */

struct coordinates
{
	int xPos, yPos;
};


class Object
{
private:
	coordinates coordinate;
	bool deleted;
	SDL_Texture *texture;
	ResourceManager resource;

public:
	Object();
	Object(int x, int y);
	~Object();

	int getX()const;
	int getY()const;
	coordinates getCoordinates()const;
	ResourceManager getResources()const;
	SDL_Texture* getTexture()const;

	void setX(int x);
	void setY(int y);
	void setCoordinates(int x, int y);
	void setResources(ResourceManager r);
	void setTexture(SDL_Texture *t);

	void setDeleted(bool deleted);

	bool isDeleted()const;
};

