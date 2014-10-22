#include "Map.h"


Map::Map()
{
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			terrain[x][y] = 0;
		}
	}
}


Map::~Map()
{
}

void Map::init(char map)
{
	stringstream filename;
	filename >> "Game/Inputs/MapData" >> map >> ".txt";
	loadMapFromFile(filename.str());
}

void Map::loadMapFromFile(string filename)
{
	// Read in the map data 
	ifstream mapData;

	//switch (filename)
	//{
	//case 0: // Custom map	
	//	mapData.open("Game/Inputs/customMap.txt", ios::in);
	//	break;
	//case 1: // Grassland map
	//	mapData.open("Game/Inputs/grassMap.txt", ios::in);
	//	break;
	//case 2: // Desert map
	//	mapData.open("Game/Inputs/desertMap.txt", ios::in);
	//	break;
	//case 3: // Tundra map
	//	mapData.open("Game/Inputs/tundraMap.txt", ios::in);
	//	break;
	//default:
	//	break;
	//}

	//int nextCheck;
	//int i = 0;

	//while (mapData >> nextCheck)
	//{
	//	passable[i] = nextCheck;
	//	i++;
	//}
	//mapData.close();
}

char Map::getTerrain(int x, int y)
{
	return terrain[x][y];
}