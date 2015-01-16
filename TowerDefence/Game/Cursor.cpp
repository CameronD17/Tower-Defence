#include "Cursor.h"

Cursor::Cursor() 
{
    setX(0);
    setY(0);
    r = b = g = 0xFF;
	setAction(0);
}

Cursor::Cursor(int x, int y) //: Object(x, y)
{
	setX(0);
	setY(0);
    r = b = g = 0xFF;
	setAction(0);
};

Cursor::~Cursor()
{

}

void Cursor::init(ResourceManager &rm)
{
	setResources(rm);
}

void Cursor::setAction(int t)
{	
	switch(t)
	{
		case 0:		//Clear tower type (blank cursor)	//WHITE
			r = g = b = 0xFF; 
			setTexture(NULL);
			currentSelection = t;
			break;
		case 1:		//Machine Gun						//BLUE
			//texture = resource.getImage("Game/Images/Sprites/tower1.png"); 
			currentSelection = t;
            break;
        case 2:		//Cannon							//YELLOW
			//texture = resource.getImage("Game/Images/Sprites/tower2.png"); 
			currentSelection = t;
            break;
        case 3:		//Laser								//PINK
			//texture = resource.getImage("Game/Images/Sprites/tower3.png"); 
			currentSelection = t;
            break;
		case 4:		//Dummy								//BROWN
			//texture = resource.getImage("Game/Images/Sprites/tower4.png");  
			currentSelection = t;
			break;
        default:
            break;
	}
}

int Cursor::getAction()const
{
	return currentSelection;
}

void Cursor::reset()
{
	setX(0);	
    setY(0);
	r = b = g = 0xFF;
	setTexture(NULL);
	setAction(0);
}

