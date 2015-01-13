#pragma once
#include "Constants.h"
#include "../Engine/Object.h"
#include "../Engine/Pathfinder.h"
#include "Map.h"
#include <vector>

#define UNCHECKED 0
#define OPEN 1
#define CLOSED 2
#define WAITING 100

struct eStats
{
	int id, targetX, targetY, currentHealth, maxHealth, value, bounty, speed;
	bool leftBase, isBoss, canSwim, isWaiting;
	string category;
};

class Enemy : public Object
{

public:
	Enemy();
	Enemy(ResourceManager &rm, int x, int y, int type, int targetX, int targetY, int level, Map* m, int i);
	~Enemy();

	Pathfinder astar;

	void update(Map* m);
	void updateTarget(int tX, int tY, Map* m); 
	void updatePath(Map* m);

	int nextMove();
	int getSpeed()const;

	bool canWalk(Map* map);
	void moveIntoNewTile(Map* m);
	void holdPosition(Map* m);
	bool checkWalkabilityOfNextTile(Map* m);
	eStats getStats()const;

private:
	eStats stats;

	int stepsTaken, stepsPerSquare;
	vector<SDL_Texture*> sprites;

	void initialise(int level, int x, int y, int tX, int tY, int type, Map* m);

	void getSprites();

	void releaseAllMyTiles(Map* m);
	void lockThisTile(Map* m);
	void lockNextTile(Map* m);

	bool reachTarget(Map* m);
};

