#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Cursor.h"
#include "SidebarButtonHandler.h"
#include <sstream>

using namespace std;

class Sidebar 
{

private:
	Engine engine;
	void toggleTowerSelectionButtonVisibility(bool b);

public:	
	Sidebar(void);
	Sidebar(Engine &e);	
	SidebarButtonHandler buttonHandler;
	
	void setup(Engine &e);
	void update(bool tower);

	int getHoveredButtonId();
	void setHoveredButton(Cursor& cursor);
};