#pragma once

#include "Cursor.h"
#include <string>

using namespace std;

class Button 
{

private:
	int x, y, width, height, fontSize, textOffset;
	bool visible, hovered, selected;

public:	
	Button(void);
	Button(int i, int x, int y, int w, int h, int f, int o, string t, bool v);		
	 
	void init(int i, int x, int y, int w, int h, int f, int o, string t, bool v);
	
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
	bool isVisible();

	bool isHovered();
	
	void select();
	void deselect();
	bool isSelected();

	bool cursorOnButton(Cursor &c);

	string text;
};
