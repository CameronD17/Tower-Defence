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
	if (getX() >= BORDER_SIZE && getX() < (BORDER_SIZE + BOARD_WIDTH) && getY() >= BORDER_SIZE && getY() < (BORDER_SIZE + BOARD_HEIGHT))
	{
		engine.graphics.drawRectangleOL(getX(), getY(), TILE_SIZE, TILE_SIZE, 0xFF, 0xFF, 0xFF);
	}
}

void Cursor::setAction(int t)
{	
	action = t;
}

int Cursor::getAction()const
{
	return action;
}

int Cursor::getCost()const
{
	switch (action)
	{
	case TOWER_1:		// Machine Gun			
		return TOWER_1_COST;
	case TOWER_2:		// Light Cannon
		return TOWER_2_COST;
	case TOWER_3:		// Sniper's Nest
		return TOWER_3_COST;
	case TOWER_4:		// Sonic Blaster
		return TOWER_4_COST;
	case TOWER_5:		// Heavy Cannon
		return TOWER_5_COST;
	case TOWER_6:		// IED
		return TOWER_6_COST;
	case TOWER_7:		// Nuclear Artillery
		return TOWER_7_COST;
	case TOWER_8:		// Ghost Hunter
		return TOWER_8_COST;
	case TOWER_9:		// Spare Tower 1
		return TOWER_9_COST;
	case TOWER_10:		// Spare Tower 2
		return TOWER_10_COST;
	default:
		return CLEAR;
	}
}

void Cursor::reset()
{
	setX(0);	
    setY(0);
	setTexture(NULL);
	setAction(0);
}