#pragma once
#include "../Engine/Constants.h"
#include "../Engine/Engine.h"
#include "Tower.h"
#include "Cursor.h"
#include "Bank.h"
#include "Notification.h"

class TowerHandler 
{		
private:
	int counter;
	Engine engine;
	char terrain[BOARD_WIDTH][BOARD_HEIGHT];

public:	
	TowerHandler(void);
		
	bool build(Map &m, Cursor &cursor, Bank &b, Notification &n);
	void sell(Map &m, int id, Bank &b, Notification &n);
	void upgrade(int id, Bank &b, Notification &n);
	void destroy();
	
	std::vector<Tower*>  towers;
	bool selected;
	tStats selectedStats;

	void init(Engine &e, Map &map);
	void draw();
};
