#pragma once
#include "Constants.h"
#include "../Engine/Engine.h"
#include <fstream>
#include <iostream>
#include <sstream>

class Map
{
public:
	Map();
	~Map();

	void init(char map);

	void loadMapFromFile(string filename);

	char getTerrain(int x, int y);

private:

	char terrain[BOARD_WIDTH][BOARD_HEIGHT];
};

