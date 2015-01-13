#include "Map.h"


Map::Map()
{
	
}


Map::~Map()
{
}


// This method is temporary, and will be extended to accept different maps based on input
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
				tiles[x][y].terrain = CLEARTERRAIN;
				tiles[x][y].enemy = 0;
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
				tiles[x][y].terrain = nextCheck;
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
	return tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].terrain;
}

int Map::getEnemy(int x, int y)
{
	return tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].enemy;
}

int Map::getTower(int x, int y)
{
	return tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].tower;
}

void Map::setTerrain(int x, int y, char m)
{
	tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].terrain = m;
}

void Map::setEnemy(int x, int y, int id)
{
	tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].enemy = id;
}

void Map::setTower(int x, int y, int id)
{
	tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].tower = id;
}

bool Map::walkable(int x, int y, int id)
{
	if ((tiles[(x / BLOCK_SIZE)][(y / BLOCK_SIZE)].terrain == BLOCKEDTERRAIN 
		|| (tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].terrain == HASENEMY)) && tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].enemy != id)
	{
		return false;
	}
	return true;
}

bool Map::buildable(int x, int y, bool waterTower)
{
	if (waterTower)
	{
		return (getTerrain(x, y) == WATERTERRAIN);
	}
	else
	{
		return (getTerrain(x, y) == CLEARTERRAIN || getTerrain(x, y) == ROUGHTERRAIN);
	}
}