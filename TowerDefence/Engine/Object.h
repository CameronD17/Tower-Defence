#pragma once
#include "../Engine/ResourceManager.h"

/*-------------------------------------------------------- *
 * OBJECT
 * Strictly speaking, this probably shouldn't be part of the
 * engine, but I needed something for the Physics class to 
 * recognise when I passed things to it
 *-------------------------------------------------------- */

struct button
{
	SDL_Rect rect;
	string name;
	bool selected;
};


class Object
{
private:
	int xPos, yPos; 
	int type;
	bool deleted;

public:	
	SDL_Texture *texture;
	ResourceManager resource;

	Object();
	Object(int x, int y);
	~Object();

	int getX()const;
	int getY()const;
	int getType()const;
	bool isDeleted()const;

	void setX(int x);
	void setY(int y);
	void setType(int type);
	void setDeleted(bool deleted);
};

