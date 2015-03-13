#pragma once
#include "../Engine/Constants.h"
#include "../Engine/Object.h"
#include "../Game/Map.h"
#include <vector>

#define UNCHECKED 0
#define OPEN 1
#define CLOSED 2
#define WAITING 100

class Pathfinder
{

public:
	Pathfinder();
	~Pathfinder();

	void initialise(int x, int y, int tX, int tY, Map m, int i, bool swim = true);

	int getID()const;
	int getMapX()const;
	int getMapY()const;
	int getNextX()const;
	int getNextY()const;
	int getNextMove()const;
	int getPathSize()const;
	int getXAt(int pos);
	int getYAt(int pos);
	void popBack();

	bool findPath(int startX, int startY, int targetX, int targetY, Map &m);

private:

	int id;
	bool canSwim;

	std::vector<int> pathToFollow;
	std::vector<int> xCoordinates;
	std::vector<int> yCoordinates;

	void setMapValues(int sX, int sY, Map m, bool swim);
	int getGCost(int x, int y, int pX, int pY);
	void binaryHeap();
	void bubbleNewF(int m);
	bool cutCorner(int a, int b);
	void calcPath();

	int openListSize, parentXval, parentYval, startX, startY, targetX, targetY;
	char terrain[BOARD_TILE_W][BOARD_TILE_H];
	int openList[BOARD_TILE_W*BOARD_TILE_H + 2];
	int whichList[BOARD_TILE_W + 1][BOARD_TILE_H + 1];
	int openX[BOARD_TILE_W*BOARD_TILE_H + 2];
	int openY[BOARD_TILE_W*BOARD_TILE_H + 2];
	int parentX[BOARD_TILE_W + 1][BOARD_TILE_H + 1];
	int parentY[BOARD_TILE_W + 1][BOARD_TILE_H + 1];
	int Fcost[BOARD_TILE_W*BOARD_TILE_H + 2];
	int Gcost[BOARD_TILE_W + 1][BOARD_TILE_H + 1];
};

