#pragma once
#include "../Game/Map.h"
#include "../Game/Constants.h"

#define PATHFOUND true
#define NOPATH false


class AStar
{
	bool findPath(int startX, int startY, int targetX, int targetY);
	void setMapValues(Map m, bool swim);

	void binaryHeap();

	bool cutCorner(int a, int b);

	int onOpenList, onClosedList, numberOfOpenListItems, parentXval, parentYval;
	
	//Create needed arrays
	char terrain[BOARD_WIDTH][BOARD_HEIGHT];

	int openList[BOARD_WIDTH*BOARD_HEIGHT + 2]; //1 dimensional array holding ID# of open list items
	int whichList[BOARD_WIDTH + 1][BOARD_HEIGHT + 1];  //2 dimensional array used to record whether a cell is on the open list or on the closed list.
	
	int openX[BOARD_WIDTH*BOARD_HEIGHT + 2]; //1d array stores the x location of an item on the open list
	int openY[BOARD_WIDTH*BOARD_HEIGHT + 2]; //1d array stores the y location of an item on the open list

	int parentX[BOARD_WIDTH + 1][BOARD_HEIGHT + 1]; //2d array to store parent of each cell (x)
	int parentY[BOARD_WIDTH + 1][BOARD_HEIGHT + 1]; //2d array to store parent of each cell (y)

	int Fcost[BOARD_WIDTH*BOARD_HEIGHT + 2];	//1d array to store F cost of a cell on the open list
	int Gcost[BOARD_WIDTH + 1][BOARD_HEIGHT + 1]; 	//2d array to store G cost for each cell.
	int Hcost[BOARD_WIDTH*BOARD_HEIGHT + 2];	//1d array to store H cost of a cell on the open list
	
};

