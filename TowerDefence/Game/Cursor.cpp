#include "Cursor.h"

Cursor::Cursor() 
{
    setX(0);
    setY(0);
    r = b = g = 0xFF;
	setType(0);
}

Cursor::Cursor(int x, int y) //: Object(x, y)
{
	setX(0);
	setY(0);
    r = b = g = 0xFF;
	setType(0);
};

Cursor::~Cursor()
{

}

void Cursor::init(ResourceManager &rm)
{
	resource = rm;
}

int Cursor::changeTowerType(int t)
{	
	switch(t){
		case 0:		//Clear tower type (blank cursor)	//WHITE
			r = g = b = 0xFF; 
			texture = NULL;
			setType(t);
			break;
		case 1:		//Machine Gun						//BLUE
			texture = resource.getImage("Game/Images/Sprites/tower1.png"); 
			setType(t);
            break;
        case 2:		//Cannon							//YELLOW
			texture = resource.getImage("Game/Images/Sprites/tower2.png"); 
			setType(t);
            break;
        case 3:		//Laser								//PINK
			texture = resource.getImage("Game/Images/Sprites/tower3.png"); 
			setType(t);
            break;
		case 4:		//Dummy								//BROWN
			texture = resource.getImage("Game/Images/Sprites/tower4.png");  
			setType(-1);
			break;
        default:
            break;
	}
	return 0;
}

void Cursor::reset()
{
	setX(0);	
    setY(0);
	r = b = g = 0xFF;
	texture = NULL;
	setType(0);
}

