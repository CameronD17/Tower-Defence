#pragma once
#include "Constants.h"
#include "../Engine/Object.h"
#include "Map.h"
#include <vector>

#define UNCHECKED 0
#define OPEN 1
#define CLOSED 2
#define WAITING 100

class Enemy : public Object
{

public:
	Enemy();
	Enemy(ResourceManager &rm, int x, int y, int type, int targetX, int targetY, int level, Map* m, int i);
	~Enemy();

	void update(Map* m);
	bool checkPathFromBase(Map* m);
	void updatePath(Map* m);
	void updateTarget(int tX, int tY);
	int nextMove();
	int getMapX()const;
	int getMapY()const;
	int getNextX()const;
	int getNextY()const;
	int getID()const;
	int getStepSize()const;
	bool canWalk(Map* map);
	void moveIntoNewTile(Map* m);
	void holdPosition(Map* m);
	bool checkWalkabilityOfNextTile(Map* m);
	int stepsTaken;

private:

	int id, targetX, targetY, currentHealth, maxHealth, value, bounty, speed, stepSize, stepsPerSquare;
	unsigned int waitingPeriod;
	bool leftBase, isBoss, canSwim, isWaiting;
	string category;
	vector<int> pathToFollow;
	vector<int> xCoordinates;
	vector<int> yCoordinates;
	vector<SDL_Texture*> sprites;

	void initialise(int level, int x, int y, int tX, int tY, int type, Map* m);

	bool findPath(int startX, int startY, Map* m);
	void getSprites();

	void releaseAllMyTiles(Map* m);
	void lockThisTile(Map* m);
	void lockNextTile(Map* m);

	bool reachTarget(Map* m);

	void aStarSetMapValues(Map* m, bool swim);
	int aStarGetGCost(int x, int y, int pX, int pY);
	void aStarBinaryHeap();
	void aStarBubbleNewF(int m);
	bool aStarCutCorner(int a, int b);
	void aStarCalcPath();

	int openListSize, parentXval, parentYval, startX, startY;
	char terrain[BOARD_WIDTH][BOARD_HEIGHT];

	int openList[BOARD_WIDTH*BOARD_HEIGHT + 2];
	int whichList[BOARD_WIDTH + 1][BOARD_HEIGHT + 1];

	int openX[BOARD_WIDTH*BOARD_HEIGHT + 2];
	int openY[BOARD_WIDTH*BOARD_HEIGHT + 2];

	int parentX[BOARD_WIDTH + 1][BOARD_HEIGHT + 1];
	int parentY[BOARD_WIDTH + 1][BOARD_HEIGHT + 1];

	int Fcost[BOARD_WIDTH*BOARD_HEIGHT + 2];
	int Gcost[BOARD_WIDTH + 1][BOARD_HEIGHT + 1];
};

