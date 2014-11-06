#pragma once
#include "Constants.h"
#include "../Engine/Object.h"
#include "Map.h"
#include <vector>

#define UNCHECKED 0
#define OPEN 1
#define CLOSED 2

class Enemy : public Object
{

public:
	Enemy();
	Enemy(ResourceManager &rm, int x, int y, int type, int targetX, int targetY, int health, int value, int bounty, Map* m, int i);
	~Enemy();

	bool checkPathFromBase(Map* m);
	void updatePath(Map* m);
	void updateTarget(int tX, int tY);
	int currentDirection(Map* m);
	int getXCoord();
	int getYCoord();
	int getID();
	int stepsTaken;

private:

	int id, targetX, targetY, currentHealth, maxHealth, value, bounty, speed;
	bool leftBase, isBoss, canSwim;
	string category;
	vector<int> pathToFollow;
	vector<int> xCoordinates;
	vector<int> yCoordinates;
	vector<SDL_Texture*> sprites;

	bool findPath(int startX, int startY, Map* m);
	void getSprites();

	void aStarSetMapValues(Map* m, bool swim);
	int aStarGetGCost(int x, int y, int pX, int pY);
	void aStarBinaryHeap();
	void aStarBubbleNewF(int m);
	bool aStarCutCorner(int a, int b);
	void aStarCalcPath();

	int openListSize, parentXval, parentYval, startX, startY;
	char terrain[BOARD_WIDTH][BOARD_HEIGHT];

	int openList[BOARD_WIDTH*BOARD_HEIGHT + 2]; //1 dimensional array holding ID# of open list items
	int whichList[BOARD_WIDTH + 1][BOARD_HEIGHT + 1];  //2 dimensional array used to record whether a cell is on the open list or on the closed list.

	int openX[BOARD_WIDTH*BOARD_HEIGHT + 2]; //1d array stores the x location of an item on the open list
	int openY[BOARD_WIDTH*BOARD_HEIGHT + 2]; //1d array stores the y location of an item on the open list

	int parentX[BOARD_WIDTH + 1][BOARD_HEIGHT + 1]; //2d array to store parent of each cell (x)
	int parentY[BOARD_WIDTH + 1][BOARD_HEIGHT + 1]; //2d array to store parent of each cell (y)

	int Fcost[BOARD_WIDTH*BOARD_HEIGHT + 2];	//1d array to store F cost of a cell on the open list
	int Gcost[BOARD_WIDTH + 1][BOARD_HEIGHT + 1]; 	//2d array to store G cost for each cell.
};

