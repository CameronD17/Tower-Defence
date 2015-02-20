#pragma once
#include "../Engine/Constants.h"
#include "../Engine/Engine.h"
#include "Enemy.h"
#include "Cursor.h"

class EnemyHandler 
{
private:
	Engine engine;
	int enemyCount;

public:	
	EnemyHandler(void);

	eStats selectedStats;
	
	void launch(Cursor &cursor, Map &m);
	void updatePaths(int x, int y, Map &m);
	void updateTargets(Map &m);
	void destroy();

	std::vector<Enemy*>  enemies;
	bool selected;

	void init(Engine &e, Map &m);
	void draw();
};
