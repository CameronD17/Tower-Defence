#pragma once

#include "Cursor.h"
#include <string>

using namespace std;

class Button 
{

private:
	int x, y, width, height;

public:	
	Button(void);
	Button(int i, int x, int y, int w, int h, string t);		
	
	void init(int i, int x, int y, int w, int h, string t);

	bool visible, hovered;

	int getX()const;
	int getY()const;
	int getW()const;
	int getH()const;
	int id;

	bool cursorOnButton(Cursor &c);

	string text;
};
