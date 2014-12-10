#pragma once

#include "../Engine/Engine.h"
#include "Constants.h"
#include "Cursor.h"
#include "Tower.h"
#include "Enemy.h"

using namespace std;

class Sidebar 
{

private:

public:	
	Sidebar(void);
	Sidebar(Engine &sc);	
	Engine engine;
};
