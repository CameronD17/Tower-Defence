#include "Map.h"


Map::Map()
{
	
}


Map::~Map()
{
}

void Map::init(char map)
{
	//stringstream filename;
	//filename >> "Game/Inputs/MapData/" >> map >> ".txt";

	if (!loadMapFromFile("Assets/Inputs/MapData/testMap.txt"))
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			for (int y = 0; y < BOARD_HEIGHT; y++)
			{
				terrain[x][y] = CLEARTERRAIN;
			}
		}
	}
}

bool Map::loadMapFromFile(string filename)
{
	// Read in the map data 
	ifstream mapData;
	
	mapData.open(filename, ios::in);

	if (mapData.good())
	{
		char nextCheck;
		int x = 0, y = 0;

		while (mapData >> nextCheck)
		{
			if (x < BOARD_WIDTH)
			{
				terrain[x][y] = nextCheck;
				x++;
			}
			else
			{
				x = 0;
				y++;
			}
		}
		mapData.close();
		return true;
	}

	return false;
}

char Map::getTerrain(int x, int y)
{
	return terrain[x / BLOCK_SIZE][y / BLOCK_SIZE];
}

int Map::getEnemy(int x, int y)
{
	return enemyPos[x / BLOCK_SIZE][y / BLOCK_SIZE];
}

void Map::setTerrain(int x, int y, char m)
{
	terrain[x / BLOCK_SIZE][y / BLOCK_SIZE] = m;
}

void Map::setEnemy(int x, int y, int id)
{
	enemyPos[x / BLOCK_SIZE][y / BLOCK_SIZE] = id;
}

bool Map::walkable(int x, int y)
{
	bool walkable = true;
	if (terrain[(x / BLOCK_SIZE)][(y / BLOCK_SIZE)] == BLOCKEDTERRAIN || terrain[x / BLOCK_SIZE][y / BLOCK_SIZE] == HASENEMY)
	{
		walkable = false;
	}
	return walkable;
}