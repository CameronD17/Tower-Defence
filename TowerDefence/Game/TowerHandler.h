#pragma once

#include "../Engine/Constants.h"
#include "Tower.h"
#include "Cursor.h"
#include "Bank.h"
using namespace std;

class TowerHandler 
{		
private:
	int towerCount;

public:	
	TowerHandler(void);

	int numTowers();
	
	bool buildTower(Map* m, Cursor &cursor, Bank* b);
	bool sellTower(Map* m, int id, Bank* b);

	void destroyObjects();
	
	vector<Tower*>  towers;

	void init(Map* map);
};
