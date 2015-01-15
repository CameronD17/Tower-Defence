#include "Button.h"

Button::Button(void){}

Button::Button(int i, int x, int y, int w, int h, string t)
{
	id = i;
	this->x = x;
	this->y = y;
	width = w;
	height = h;
	text = t;
	visible = false;
	hovered = false;
}

void Button::init(int i, int x, int y, int w, int h, string t)
{
	id = i;
	this->x = x;
	this->y = y;
	width = w;
	height = h;
	text = t;
	visible = false;
	hovered = false;
}

int Button::getX()const
{
	return x;
}

int Button::getY()const
{
	return y;
}

int Button::getW()const
{
	return width;
}

int Button::getH()const
{
	return height;
}

bool Button::cursorOnButton(Cursor &c)
{
	if (c.getX() > x && c.getX() < (x + width) && c.getY() > y && c.getY() < (y + height))
	{
		hovered = true;
		return true;
	}

	hovered = false;
	return false;
}