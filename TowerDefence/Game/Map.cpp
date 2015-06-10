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

	for (int x = 0; x < BOARD_TILE_W; x++)
	{
		for (int y = 0; y < BOARD_TILE_H; y++)
		{
			tiles[x][y].terrain = CLEAR_TERRAIN;
			tiles[x][y].enemy = NO_ENEMY;
			tiles[x][y].tower = NO_TOWER;
			tiles[x][y].isEnemy = false;
			tiles[x][y].isTower = false;
		}
	}

	startX = 24, startY = 24, targetX = 312, targetY = 408;

	if (!loadMapFromFile(filename.str()))
	{
		for (int x = 0; x < BOARD_TILE_W; x++)
		{
			for (int y = 0; y < BOARD_TILE_H; y++)
			{
				tiles[x][y].terrain = CLEAR_TERRAIN;
				tiles[x][y].enemy = NO_ENEMY;
				tiles[x][y].tower = NO_TOWER;
				tiles[x][y].isEnemy = false;
				tiles[x][y].isTower = false;
			}
		}
	}
}

void Map::draw()
{
	// TODO: update this method with actual imagery
	for (int x = 0; x < BOARD_WIDTH; x += TILE_SIZE)
	{
		for (int y = 0; y < BOARD_HEIGHT; y += TILE_SIZE)
		{
			if (getTerrain(x, y) == BLOCKED_TERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER_SIZE, y + BORDER_SIZE, TILE_SIZE, TILE_SIZE, 255, 255, 0);
			}
			else if (getTerrain(x, y) == WATER_TERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER_SIZE, y + BORDER_SIZE, TILE_SIZE, TILE_SIZE, 0, 0, 255);
			}
			else if (getTerrain(x, y) == ROUGH_TERRAIN)
			{
				engine.graphics.drawRectangle(x + BORDER_SIZE, y + BORDER_SIZE, TILE_SIZE, TILE_SIZE, 0, 255, 0);
			}
		}
	}

	//Target
	engine.graphics.drawRectangleOL(targetX - 1, targetY - 1, TILE_SIZE + 2, TILE_SIZE + 2, 0, 255, 255);
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
			if (x < BOARD_TILE_W)
			{
				tiles[x][y].terrain = nextCheck;
				tiles[x][y].enemy = NO_ENEMY;
				tiles[x][y].tower = NO_TOWER;
				tiles[x][y].isEnemy = false;
				tiles[x][y].isTower = false;
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
	return tiles[x / TILE_SIZE][y / TILE_SIZE].terrain;
}

int Map::getEnemy(int x, int y)
{
	return tiles[x / TILE_SIZE][y / TILE_SIZE].enemy;
}

int Map::getTower(int x, int y)
{
	return tiles[x / TILE_SIZE][y / TILE_SIZE].tower;
}

void Map::setTerrain(int x, int y, char m)
{
	tiles[x / TILE_SIZE][y / TILE_SIZE].terrain = m;
}

void Map::setEnemy(int x, int y, int id)
{
	tiles[x / TILE_SIZE][y / TILE_SIZE].enemy = id;
	tiles[x / TILE_SIZE][y / TILE_SIZE].isEnemy = (id != NO_ENEMY) ? true : false;
}

void Map::setTower(int x, int y, int id)
{
	tiles[x / TILE_SIZE][y / TILE_SIZE].tower = id;
	tiles[x / TILE_SIZE][y / TILE_SIZE].isTower = (id != NO_TOWER) ? true : false;
}

bool Map::hasEnemy(int x, int y)
{
	return tiles[x / TILE_SIZE][y / TILE_SIZE].isEnemy;
}

bool Map::hasTower(int x, int y)
{
	return tiles[x / TILE_SIZE][y / TILE_SIZE].isTower;
}

bool Map::walkable(int x, int y, int id, int next)
{
	if ((tiles[(x / TILE_SIZE)][(y / TILE_SIZE)].terrain == BLOCKED_TERRAIN 
		|| (tiles[x / TILE_SIZE][y / TILE_SIZE].isEnemy) 
		|| (tiles[x / TILE_SIZE][y / TILE_SIZE].isTower)) 
		&& tiles[x / TILE_SIZE][y / TILE_SIZE].enemy != id)
	{
		return false;
	}
	return true;

	/*bool walkable = true;

	x /= TILE_SIZE; y /= TILE_SIZE;
		
	switch (next)
	{
	case UP: case LEFT: case RIGHT: case DOWN:
		if ((tiles[x][y].terrain == BLOCKED_TERRAIN || tiles[x][y].isEnemy || tiles[x][y].isTower) && tiles[x][y].enemy != id)
		{
			walkable = false;
		}
		break;
	case UP_LEFT:
		if (((tiles[x][y].terrain == BLOCKED_TERRAIN || tiles[x][y].isEnemy || tiles[x][y].isTower) && tiles[x][y].enemy != id)
			&& ((tiles[x - TILE_SIZE][y].terrain == BLOCKED_TERRAIN || tiles[x - TILE_SIZE][y].isEnemy || tiles[x - TILE_SIZE][y].isTower) && tiles[x - TILE_SIZE][y].enemy != id)
			&& ((tiles[x][y + TILE_SIZE].terrain == BLOCKED_TERRAIN || tiles[x][y + TILE_SIZE].isEnemy || tiles[x][y + TILE_SIZE].isTower) && tiles[x][y + TILE_SIZE].enemy != id))
		{
			walkable = false;
		}
		break;
	case UP_RIGHT:
		if (((tiles[x][y].terrain == BLOCKED_TERRAIN || tiles[x][y].isEnemy || tiles[x][y].isTower) && tiles[x][y].enemy != id)
			&& ((tiles[x + TILE_SIZE][y].terrain == BLOCKED_TERRAIN || tiles[x + TILE_SIZE][y].isEnemy || tiles[x + TILE_SIZE][y].isTower) && tiles[x + TILE_SIZE][y].enemy != id)
			&& ((tiles[x][y + TILE_SIZE].terrain == BLOCKED_TERRAIN || tiles[x][y + TILE_SIZE].isEnemy || tiles[x][y + TILE_SIZE].isTower) && tiles[x][y + TILE_SIZE].enemy != id))
		{
			walkable = false;
		}
		break;
	case DOWN_LEFT:
		if (((tiles[x][y].terrain == BLOCKED_TERRAIN || tiles[x][y].isEnemy || tiles[x][y].isTower) && tiles[x][y].enemy != id)
			&& ((tiles[x + TILE_SIZE][y].terrain == BLOCKED_TERRAIN || tiles[x + TILE_SIZE][y].isEnemy || tiles[x + TILE_SIZE][y].isTower) && tiles[x + TILE_SIZE][y].enemy != id)
			&& ((tiles[x][y - TILE_SIZE].terrain == BLOCKED_TERRAIN || tiles[x][y - TILE_SIZE].isEnemy || tiles[x][y - TILE_SIZE].isTower) && tiles[x][y - TILE_SIZE].enemy != id))
		{
			walkable = false;
		}
		break;
	case DOWN_RIGHT:
		if (((tiles[x][y].terrain == BLOCKED_TERRAIN || tiles[x][y].isEnemy || tiles[x][y].isTower) && tiles[x][y].enemy != id)
			&& ((tiles[x - TILE_SIZE][y].terrain == BLOCKED_TERRAIN || tiles[x - TILE_SIZE][y].isEnemy || tiles[x - TILE_SIZE][y].isTower) && tiles[x - TILE_SIZE][y].enemy != id)
			&& ((tiles[x][y - TILE_SIZE].terrain == BLOCKED_TERRAIN || tiles[x][y - TILE_SIZE].isEnemy || tiles[x][y - TILE_SIZE].isTower) && tiles[x][y - TILE_SIZE].enemy != id))
		{
			walkable = false;
		}
		break;
	}

	return walkable;*/
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