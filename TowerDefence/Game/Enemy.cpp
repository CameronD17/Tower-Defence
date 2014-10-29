#include "Enemy.h"

Enemy::Enemy()
{
    setX(0);
    setY(0);
	setType(0);
	targetX = 0;
	targetY = 0;
	currentHealth = 30;
	value = 150;
	bounty = 20;
	stepsTaken = 0;
}

Enemy::Enemy(ResourceManager &rm, int x, int y, int t, int tX, int tY, int hea, int val, int bou){
    setX(x);
    setY(y);
	setType(t);
	targetX = tX;
	targetY = tY;
	maxHealth = hea;
	currentHealth = hea;
	value = val;
	bounty = bou;
	pathToFollow = makeTestPath();
	newPath = true;
	leftBase = false;
	resource = rm;
	getSprites();
	location = 0;
	stepsTaken = 0;
}

Enemy::~Enemy()
{

}

std::vector<int> Enemy::makeTestPath()
{
	std::vector<int> testP;
	for (int i = 0; i < 17; i++)
	{
		testP.push_back(3);		
		testP.push_back(2);
	}
	testP.push_back(2);
	testP.push_back(2);

	return testP;
}

void Enemy::getSprites()
{
	switch(getType())
	{
	case 1: default:
		sprites.push_back(resource.getImage("Game/Images/Sprites/enemyUp.png"));
		sprites.push_back(resource.getImage("Game/Images/Sprites/enemyDown.png"));
		sprites.push_back(resource.getImage("Game/Images/Sprites/enemyLeft.png"));
		sprites.push_back(resource.getImage("Game/Images/Sprites/enemyRight.png"));
		break;
	case 2:
		sprites.push_back(resource.getImage("Game/Images/Sprites/jetUp.png"));
		sprites.push_back(resource.getImage("Game/Images/Sprites/jetDown.png"));
		sprites.push_back(resource.getImage("Game/Images/Sprites/jetLeft.png"));
		sprites.push_back(resource.getImage("Game/Images/Sprites/jetRight.png"));
		break;
	}
}

void Enemy::locationTracker()
{
	if(location < BLOCK_SIZE)
	{
		location += BULLET_SIZE;
	}
	else
	{
		location = 0;
		pathToFollow.pop_back();
	}
}