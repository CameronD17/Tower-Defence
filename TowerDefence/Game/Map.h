#pragma once
#include "../Engine/Constants.h"
#include "../Engine/Engine.h"

struct Tile 
{
	char terrain;
	int enemy;
	int tower;
	bool isEnemy;
	bool isTower;
};

class Map
{
private:
	Engine engine;
	Tile tiles[BOARD_WIDTH][BOARD_HEIGHT];
	bool loadMapFromFile(std::string filename);

public:
	Map();
	~Map();

	int startX, startY, targetX, targetY;
	
	void init(Engine &e, std::string map);
	void draw();

	char getTerrain(int x, int y);
	int getEnemy(int x, int y);
	int getTower(int x, int y);

	void setTerrain(int x, int y, char m);
	void setEnemy(int x, int y, int id);
	void setTower(int x, int y, int id);

	bool hasEnemy(int x, int y);
	bool hasTower(int x, int y);

	bool walkable(int x, int y, int id, int next = UP);
	bool buildable(int x, int y, bool waterTower = false);
};

