#pragma once
#include "../Engine/Constants.h"
#include "../Engine/Engine.h"
#include "Enemy.h"
#include "Cursor.h"
#include <iostream>
#include <iomanip>

struct Wave
{
	int numberOfEnemies;
	int type;
	int level;
	bool isBoss;
};

class EnemyHandler 
{
private:
	Engine engine;
	int enemyCount;
	bool loadEnemiesFromFile(Map &m, std::string filename);
	int launched;
	bool nextWaveWaiting;
	unsigned int timer, waveTimer;
	std::vector<Enemy*> awaitinglaunch;

public:	
	EnemyHandler(void);

	//eStats selectedStats;
	bool autolaunch;
	
	void launch(Map &m);
	void launch(Cursor &cursor, Map &m);
	void autoLaunch(Map &m);
	void nextWave();
	void updatePaths(int x, int y, Map &m);
	void updateTargets(Map &m);
	void destroy();

	std::vector<Enemy*>  enemies;
	std::vector<Wave>    waves;
	//bool selected;
	int waveCount;

	void init(Engine &e, Map &m, std::string g);
	void draw();
};
