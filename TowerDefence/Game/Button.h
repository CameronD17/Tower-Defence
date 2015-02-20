#pragma once
#include "Cursor.h"

class Button 
{
private:
	int x, y, width, height, fontSize, textOffset;
	bool visible, hovered, selected;

public:	
	Button(void);
	Button(int i, int x, int y, int w, int h, int f, int o, std::string t, bool v);
	 
	void init(int i, int x, int y, int w, int h, int f, int o, std::string t, bool v);
	
	int getX()const;
	int getY()const;
	int getW()const;
	int getH()const;
	int getFontSize()const;
	int getOffset()const;
	int getHoveredButtonId();
	int id;

	void hide();
	void show();

	void select();
	void deselect();

	bool isVisible();
	bool isHovered();
	bool isSelected();

	bool cursorOnButton(Cursor &c);

	std::string text;
};
