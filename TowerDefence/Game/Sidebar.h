#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Constants.h"
#include "Cursor.h"
#include "ButtonHandler.h"
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
	ButtonHandler buttonHandler;
	
	void setup(Engine &e);
	void update(bool tower);
};