#pragma once
#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Board.h"
#include "Cursor.h"
#include "ButtonHandler.h"
#include "Notification.h"

class Sidebar 
{
private:
	Engine engine;
	ButtonHandler buttonHandler;
	
public:	
	Sidebar(void);
	
	void setup(Engine &e);
	int getInput(input k, Cursor &c, Board &b, Notification &n);
	void update(Board &b);
	void draw();
};