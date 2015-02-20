#include "Map.h"

Map::Map()
{	
}

Map::~Map()
{
}

void Map::init(Engine& e, std::string map)
{
	engine = e;

	std::stringstream filename;
	filename << map << "map.txt";


	if (!loadMapFromFile(filename.str()))
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			for (int y = 0; y < BOARD_HEIGHT; y++)
			{
				tiles[x][y].terrain = CLEAR_TERRAIN;
				tiles[x][y].enemy = NO_ENEMY;
				tiles[x][y].tower = NO_TOWER;
			}
		}

		startX = 24, startY = 24, targetX = 312, targetY = 408;
	}

	startX = 24, startY = 24, targetX = 312, targetY = 408;
}

void Map::draw()
{
	// TODO: update this method with actual imagery
	for (int x = 0; x < BOARD_WIDTH*BLOCK_SIZE; x += BLOCK_SIZE)
	{
		for (int y = 0; y < BOARD_HEIGHT*BLOCK_SIZE; y += BLOCK_SIZE)
		{
			if (getTerrain(x, y) == BLOCKED_TERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER_SIZE, y + BORDER_SIZE, BLOCK_SIZE, BLOCK_SIZE, 255, 255, 0);
			}
			else if (getTerrain(x, y) == WATER_TERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER_SIZE, y + BORDER_SIZE, BLOCK_SIZE, BLOCK_SIZE, 0, 0, 255);
			}
			else if (getTerrain(x, y) == ROUGH_TERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER_SIZE, y + BORDER_SIZE, BLOCK_SIZE, BLOCK_SIZE, 0, 255, 0);
			}
		}
	}

	Uint8 red = 154, green = 0, blue = 0;
	engine.graphics.drawRectangle(0, 0, BORDER_SIZE, ((BORDER_SIZE * 2) + (BOARD_HEIGHT * BLOCK_SIZE)), red, green, blue); // Left
	engine.graphics.drawRectangle(BORDER_SIZE + (BOARD_WIDTH * BLOCK_SIZE), 0, BORDER_SIZE, ((BORDER_SIZE * 2) + (BOARD_HEIGHT * BLOCK_SIZE)), red, green, blue); //Right
	engine.graphics.drawRectangle(0, 0, BORDER_SIZE + (BOARD_WIDTH * BLOCK_SIZE), BORDER_SIZE, red, green, blue); // Top
	engine.graphics.drawRectangle(0, BORDER_SIZE + (BOARD_HEIGHT*BLOCK_SIZE), BORDER_SIZE + (BOARD_WIDTH * BLOCK_SIZE), BORDER_SIZE, red, green, blue);	// Bottom
	
	//Target
	engine.graphics.drawRectangleOL(targetX - 1, targetY - 1, BLOCK_SIZE + 2, BLOCK_SIZE + 2, 0, 255, 255);
}

bool Map::loadMapFromFile(std::string filename)
{
	std::ifstream mapData;
	
	mapData.open(filename);

	if (mapData.good())
	{
		char nextCheck;
		int x = 0, y = 0;

		while (mapData >> nextCheck)
		{
			if (x < BOARD_WIDTH)
			{
				tiles[x][y].terrain = nextCheck;
				tiles[x][y].enemy = NO_ENEMY;
				tiles[x][y].tower = NO_TOWER;
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


	// ADD TO FILE READIN
	startX = 24, startY = 24, targetX = 384, targetY = 408;

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
	tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].isEnemy = (id != NO_ENEMY) ? true : false;
}

void Map::setTower(int x, int y, int id)
{
	tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].tower = id;
	tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].isTower = (id != NO_TOWER) ? true : false;
}

bool Map::hasEnemy(int x, int y)
{
	return tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].isEnemy;
}

bool Map::hasTower(int x, int y)
{
	return tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].isTower;
}

bool Map::walkable(int x, int y, int id)
{
	if ((tiles[(x / BLOCK_SIZE)][(y / BLOCK_SIZE)].terrain == BLOCKED_TERRAIN 
		|| (tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].isEnemy) 
		|| (tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].isTower)) 
		&& tiles[x / BLOCK_SIZE][y / BLOCK_SIZE].enemy != id)
	{
		return false;
	}
	return true;
}

bool Map::buildable(int x, int y, bool waterTower)
{
	if (waterTower)
	{
		return (getTerrain(x, y) == WATER_TERRAIN);
	}
	else
	{
		return (getTerrain(x, y) == CLEAR_TERRAIN || getTerrain(x, y) == ROUGH_TERRAIN);
	}
}