#include "Cursor.h"

Cursor::Cursor() 
{
    setX(0);
    setY(0);
	setAction(0);
}

Cursor::Cursor(int x, int y) //: Object(x, y)
{
	setX(0);
	setY(0);
	setAction(0);	
};

Cursor::~Cursor()
{

}

void Cursor::init(Engine &e)
{
	engine = e;
	setResources(engine.resources);
}

void Cursor::draw()
{
	if (getX() >= BORDER_SIZE && getX() < (BORDER_SIZE + (BOARD_WIDTH*BLOCK_SIZE)) && getY() >= BORDER_SIZE && getY() < (BORDER_SIZE + (BOARD_HEIGHT*BLOCK_SIZE)))
	{
		engine.graphics.drawRectangleOL(getX(), getY(), BLOCK_SIZE, BLOCK_SIZE, 0xFF, 0xFF, 0xFF);
	}
}

void Cursor::setAction(int t)
{	
	action = t;
	towerStats.type = t; 
	towerStats.id = -1;
	towerStats.maxCapacity = 1;
	towerStats.level = 0;
	towerStats.maxLevel = 3;
	towerStats.hits = 0;
	towerStats.kills = 0;

	switch(t)
	{
		case TOWER_1:		// Machine Gun			
			towerStats.cost = 50;
			towerStats.damage = 4;
			towerStats.range = 4 * BLOCK_SIZE;
			towerStats.reload = 20;
			//texture = resource.getImage("Game/Images/Sprites/tower1.png"); 
            break;
        case TOWER_2:		// Light Cannon
			towerStats.cost = 75;
			towerStats.damage = 6;
			towerStats.range = 6 * BLOCK_SIZE;
			towerStats.reload = 40;
			//texture = resource.getImage("Game/Images/Sprites/tower2.png"); 
            break;
        case TOWER_3:		// Sniper's Nest
			towerStats.cost = 90;
			towerStats.damage = 15;
			towerStats.range = 10 * BLOCK_SIZE;
			towerStats.reload = 120;
			//texture = resource.getImage("Game/Images/Sprites/tower3.png"); 
            break;
		case TOWER_4:		// Sonic Blaster
			towerStats.cost = 60;
			towerStats.damage = 6;
			towerStats.range = 6 * BLOCK_SIZE;
			towerStats.reload = 40;
			//texture = resource.getImage("Game/Images/Sprites/tower4.png");  
			break;
		case TOWER_5:		// Heavy Cannon
			towerStats.cost = 110;
			towerStats.damage = 15;
			towerStats.range = 8 * BLOCK_SIZE;
			towerStats.reload = 80;
			//texture = resource.getImage("Game/Images/Sprites/tower5.png");  
			break;
		case TOWER_6:		// IED
			towerStats.cost = 25;
			towerStats.damage = 20;
			towerStats.range = 1 * BLOCK_SIZE;
			towerStats.reload = 0;
			//texture = resource.getImage("Game/Images/Sprites/tower6.png");  
			break;
		case TOWER_7:		// Nuclear Artillery
			towerStats.cost = 140;
			towerStats.damage = 30;
			towerStats.range = 4 * BLOCK_SIZE;
			towerStats.reload = 80;
			//texture = resource.getImage("Game/Images/Sprites/tower7.png");  
			break;
		case TOWER_8:		// Ghost Hunter
			towerStats.cost = 175;
			towerStats.damage = 25;
			towerStats.range = 4 * BLOCK_SIZE;
			towerStats.reload = 40;
			//texture = resource.getImage("Game/Images/Sprites/tower8.png");  
			break;
		case TOWER_9:		// Spare Tower 1
			towerStats.cost = 0;
			towerStats.damage = 0;
			towerStats.range = 0 * BLOCK_SIZE;
			towerStats.reload = 0;
			//texture = resource.getImage("Game/Images/Sprites/tower9.png");  
			break;
		case TOWER_10:		// Spare Tower 2
			towerStats.cost = 0;
			towerStats.damage = 0;
			towerStats.range = 0 * BLOCK_SIZE;
			towerStats.reload = 0;
			//texture = resource.getImage("Game/Images/Sprites/tower10.png");  
			break;
        default:
			setTexture(NULL);
			//towerSelected = false;
            break;
	}
}

int Cursor::getAction()const
{
	return action;
}

tStats Cursor::getStats()const
{
	return towerStats;
}

void Cursor::reset()
{
	setX(0);	
    setY(0);
	setTexture(NULL);
	setAction(0);
}