#pragma once

#include "../Engine/Constants.h"
#include "../Engine/Engine.h"
#include "Enemy.h"
#include "Cursor.h"

using namespace std;

class EnemyHandler 
{

public:	
	EnemyHandler(void);

	int enemyCount;

	Engine engine;
	
	// Gameplay methods
	void launchEnemy(Cursor &cursor, Map &m);
	void updateEnemyPaths(int x, int y, Map &m);
	void updateEnemyTargets(Map &m);
	void destroyObjects();

	vector<Enemy*>  enemies;

	void init(Engine& e, Map &m);
};
