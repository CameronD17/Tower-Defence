#pragma once

#include "../Engine/Constants.h"
#include "Tower.h"
#include "Cursor.h"
using namespace std;

class TowerHandler 
{

private:
	// Per-game values				
	int credit;
		
public:	
	TowerHandler(void);

	int towerCount;
	
	bool buildTower(Cursor &cursor, Map* m);
	bool sellTower(Map* m, int id);

	void destroyObjects();
	
	vector<Tower*>  towers;

	void init(Map* map);
};
