#include "Button.h"

Button::Button(void){}

Button::Button(int i, int x, int y, int w, int h, int f, int o, string t, bool v)
{
	id = i;
	this->x = x;
	this->y = y;
	width = w;
	height = h;
	fontSize = f;
	textOffset = o;
	text = t;
	visible = v;
	hovered = false;
	selected = false;
}

void Button::init(int i, int x, int y, int w, int h, int f, int o, string t, bool v)
{
	id = i;
	this->x = x;
	this->y = y;
	width = w;
	height = h;
	fontSize = f;
	textOffset = o;
	text = t;
	visible = v;
	hovered = false;
	selected = false;
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

int Button::getFontSize()const
{
	return fontSize;
}

int Button::getOffset()const
{
	return textOffset;
}

void Button::hide()
{
	visible = false;
}

void Button::show()
{
	visible = true;
}

bool Button::isVisible()
{
	return visible;
}

bool Button::isHovered()
{
	return hovered;
}

bool Button::isSelected()
{
	return selected;
}

void Button::select()
{
	selected = true;
}

void Button::deselect()
{
	selected = false;
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