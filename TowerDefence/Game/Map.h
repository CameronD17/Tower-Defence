#pragma once
#include "../Engine/Constants.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

struct Tile {
	char terrain;
	int enemy;
	int tower;
	bool isEnemy;
	bool isTower;
};


class Map
{
public:
	Map();
	~Map();

	int startX, startY, targetX, targetY;
	
	void init(string map);
	bool loadMapFromFile(string filename);

	char getTerrain(int x, int y);
	int getEnemy(int x, int y);
	int getTower(int x, int y);

	void setTerrain(int x, int y, char m);
	void setEnemy(int x, int y, int id);
	void setTower(int x, int y, int id);

	bool hasEnemy(int x, int y);
	bool hasTower(int x, int y);

	bool walkable(int x, int y, int id = 0);
	bool buildable(int x, int y, bool waterTower = false);

private:

	Tile tiles[BOARD_WIDTH][BOARD_HEIGHT];
};

