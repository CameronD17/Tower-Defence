#pragma once
#include "Constants.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

struct Tile {
	char terrain;
	int enemy;
};


class Map
{
public:
	Map();
	~Map();

	void init(char map);
	bool loadMapFromFile(string filename);

	char getTerrain(int x, int y);
	int getEnemy(int x, int y);

	void setTerrain(int x, int y, char m);
	void setEnemy(int x, int y, int id);

	bool walkable(int x, int y, int id = 0);
	bool buildable(int x, int y, bool waterTower = false);

private:

	Tile tiles[BOARD_WIDTH][BOARD_HEIGHT];
};

