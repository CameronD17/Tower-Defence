#pragma once
#include "Constants.h"
#include "../Engine/Object.h"
#include <vector>

class Enemy : public Object
{

public:
	int targetX, targetY, health, maxHealth, value, bounty, loc;

	std::vector<int>  pathToFollow;
	std::vector<SDL_Texture*> sprites;

	bool newPath, leftBase;

	void locationTracker();

	Enemy();
	Enemy(ResourceManager &rm, int x, int y, int type, int targetX, int targetY, int health, int value, int bounty);
	~Enemy();

private:
	void findPath();
	void getSprites();
	std::vector<int> makeTestPath();
};

