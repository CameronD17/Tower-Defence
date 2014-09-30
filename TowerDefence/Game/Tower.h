#pragma once
#include "Constants.h"
#include "Bullet.h"
#include "../Engine/Object.h"
#include <vector>

using namespace std;

class Tower: public Object
{

public:
	Tower();
	Tower(int x, int y); 
	Tower(int x, int y, int type, SDL_Texture *tex);
	~Tower();	

	vector<Bullet*>  bullets;

	bool antiGround;
	bool antiAir;

	void fire(int dir);
	void levelUp();

	int damage, reloadRate, level;
	unsigned int timer;
	
private:
	int killCount;
	void getTargets();

};


